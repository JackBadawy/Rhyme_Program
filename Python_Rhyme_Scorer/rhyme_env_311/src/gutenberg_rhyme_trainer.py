import torch
from torch.utils.data import Dataset, DataLoader
import requests
import re
from tqdm import tqdm
import random

class PoemRhymeExtractor:
    def __init__(self):
        self.end_words = []
        self.rhyme_pairs = []
        
    def extract_end_words(self, text):
        """Extract words at the end of each line"""
        lines = [line.strip() for line in text.split('\n') if line.strip()]
        
        end_words = [re.sub(r'[^\w\s]', '', line.split()[-1].lower()) 
                    for line in lines if line.split()]
        return [word for word in end_words if word]  
    
    def find_rhyme_pairs(self, end_words, window_size=4):
        """Find potential rhyming pairs within a window of lines"""
        pairs = []
        for i in range(len(end_words)-1):
            for j in range(i+1, min(i+window_size+1, len(end_words))):
                
                if end_words[i] != end_words[j]:
                    pairs.append((end_words[i], end_words[j]))
        return pairs

    def process_poem(self, text):
        """Process a single poem"""
        end_words = self.extract_end_words(text)
        self.end_words.extend(end_words)
        self.rhyme_pairs.extend(self.find_rhyme_pairs(end_words))

class GutenbergPoetryDataset(Dataset):
    def __init__(self, word_vectors, rhyme_pairs, neg_ratio=1.0):
        self.word_vectors = word_vectors
        self.rhyme_pairs = []
        self.labels = []
        
        
        for word1, word2 in rhyme_pairs:
            if word1 in word_vectors and word2 in word_vectors:
                self.rhyme_pairs.append((word1, word2))
                self.labels.append(1.0)
        
        
        vocab_list = list(word_vectors.key_to_index.keys())
        num_neg = int(len(self.rhyme_pairs) * neg_ratio)
        for _ in range(num_neg):
            word1 = random.choice(vocab_list)
            word2 = random.choice(vocab_list)
            if word1 != word2:
                self.rhyme_pairs.append((word1, word2))
                self.labels.append(0.0)
    
    def __len__(self):
        return len(self.rhyme_pairs)
    
    def __getitem__(self, idx):
        word1, word2 = self.rhyme_pairs[idx]
        vec1 = torch.tensor(self.word_vectors[word1], dtype=torch.float32)
        vec2 = torch.tensor(self.word_vectors[word2], dtype=torch.float32)
        combined = torch.cat([vec1, vec2])
        return combined, torch.tensor([self.labels[idx]], dtype=torch.float32)

def train_on_gutenberg_poetry(model, word_vectors, num_poems=5, epochs=3, batch_size=32):
    """Train the rhyme model on Project Gutenberg poetry"""
    
    poem_urls = [
        'https://www.gutenberg.org/cache/epub/1065/pg1065.txt',  
        'https://www.gutenberg.org/files/19860/19860-0.txt',    
        'https://www.gutenberg.org/cache/epub/2361/pg2361.txt', 
    ]
    
    extractor = PoemRhymeExtractor()
    
    print("Collecting poetry data...")
    for url in tqdm(poem_urls[:num_poems]):
        try:
            response = requests.get(url)
            response.raise_for_status()
            text = response.text
            extractor.process_poem(text)
        except Exception as e:
            print(f"Error processing {url}: {str(e)}")
            continue
    
    print(f"Found {len(extractor.rhyme_pairs)} potential rhyme pairs")
    
    
    dataset = GutenbergPoetryDataset(word_vectors, extractor.rhyme_pairs)
    dataloader = DataLoader(dataset, batch_size=batch_size, shuffle=True)
    
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    optimizer = torch.optim.Adam(model.parameters())
    criterion = torch.nn.MSELoss()
    
    print("Training model...")
    model.train()
    for epoch in range(epochs):
        total_loss = 0
        for X, y in tqdm(dataloader, desc=f"Epoch {epoch+1}/{epochs}"):
            X, y = X.to(device), y.to(device)
            
            optimizer.zero_grad()
            outputs = model(X)
            loss = criterion(outputs, y)
            loss.backward()
            optimizer.step()
            
            total_loss += loss.item()
        
        avg_loss = total_loss / len(dataloader)
        print(f"Epoch {epoch+1}, Average Loss: {avg_loss:.4f}")
    
    return model


if __name__ == "__main__":
    from rhyme_quality_scorer import RhymeEmbeddingNet
    import gensim.downloader as api
    
    
    print("Loading word embeddings...")
    word_vectors = api.load('glove-twitter-25')
    
    
    model = RhymeEmbeddingNet()
    model = train_on_gutenberg_poetry(model, word_vectors, num_poems=2, epochs=3)
    
    
    torch.save(model.state_dict(), 'rhyme_model_gutenberg.pth')