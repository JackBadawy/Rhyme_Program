import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy as np
from torch.utils.data import Dataset, DataLoader

class RhymeEmbeddingNet(nn.Module):
    def __init__(self, embedding_dim=25):
        super().__init__()
        self.fc1 = nn.Linear(embedding_dim * 2, 64)
        self.fc2 = nn.Linear(64, 32)
        self.fc3 = nn.Linear(32, 1)
        self.dropout = nn.Dropout(0.2)
        
    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = self.dropout(x)
        x = F.relu(self.fc2(x))
        x = self.dropout(x)
        x = torch.sigmoid(self.fc3(x))
        return x

class RhymeDataset(Dataset):
    def __init__(self, word_pairs, word_vectors):
        self.word_pairs = word_pairs
        self.word_vectors = word_vectors
        
    def __len__(self):
        return len(self.word_pairs)
        
    def __getitem__(self, idx):
        word1, word2, score = self.word_pairs[idx]
        try:
            vec1 = torch.tensor(self.word_vectors[word1.lower()], dtype=torch.float32)
            vec2 = torch.tensor(self.word_vectors[word2.lower()], dtype=torch.float32)
            combined = torch.cat([vec1, vec2])
            return combined, torch.tensor([score], dtype=torch.float32)
        except KeyError:
            
            return torch.zeros(50, dtype=torch.float32), torch.tensor([0.5], dtype=torch.float32)

class PyTorchRhymeScorer:
    def __init__(self):
        """Initialize the scorer with word embeddings and neural network"""
        print("Loading word embeddings...")
        import gensim.downloader as api
        self.word_vectors = api.load('glove-twitter-25')
        print("Word embeddings loaded")
        
        
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.model = RhymeEmbeddingNet().to(self.device)
        
        
        try:
            self.model.load_state_dict(torch.load('rhyme_model.pth'))
            self.model.eval()
        except FileNotFoundError:
            print("No pre-trained model found, training new model...")
            self._train_model()
            
    def _train_model(self, epochs=10):
        """Train the neural network on sample rhyming data"""
        
        training_pairs = [
            ("light", "bright", 0.9),
            ("light", "site", 0.7),
            ("blue", "true", 0.9),
            ("blue", "zoo", 0.6),
            
        ]
        
        dataset = RhymeDataset(training_pairs, self.word_vectors)
        dataloader = DataLoader(dataset, batch_size=8, shuffle=True)
        
        optimizer = torch.optim.Adam(self.model.parameters())
        criterion = nn.MSELoss()
        
        self.model.train()
        for epoch in range(epochs):
            total_loss = 0
            for X, y in dataloader:
                X, y = X.to(self.device), y.to(self.device)
                optimizer.zero_grad()
                outputs = self.model(X)
                loss = criterion(outputs, y)
                loss.backward()
                optimizer.step()
                total_loss += loss.item()
            
            print(f"Epoch {epoch+1}, Loss: {total_loss/len(dataloader):.4f}")
            
        torch.save(self.model.state_dict(), 'rhyme_model.pth')
        self.model.eval()
    def train_on_gutenberg(self, num_poems=2, epochs=3):
        """Train the model on Project Gutenberg poetry data"""
        from gutenberg_rhyme_trainer import train_on_gutenberg_poetry
        self.model = train_on_gutenberg_poetry(
            self.model,
            self.word_vectors,
            num_poems=num_poems,
            epochs=epochs
        )
        torch.save(self.model.state_dict(), 'rhyme_model_gutenberg.pth')

    def get_combined_score(self, base_word, rhyme_word, phonetic_score):
        """Calculate combined score using neural network"""
        try:
            vec1 = torch.tensor(self.word_vectors[base_word.lower()], dtype=torch.float32)
            vec2 = torch.tensor(self.word_vectors[rhyme_word.lower()], dtype=torch.float32)
            combined = torch.cat([vec1, vec2]).unsqueeze(0).to(self.device)
            
            with torch.no_grad():
                semantic_score = self.model(combined).item()
            
            combined_score = (0.7 * phonetic_score) + (0.3 * semantic_score)
            return combined_score
            
        except KeyError:
            return phonetic_score
        
    def rank_rhymes(self, base_word, rhyme_candidates):
        """Rank rhyme candidates based on neural network scores"""
        scored_rhymes = []
        
        for rhyme_word, phonetic_score in rhyme_candidates:
            combined_score = self.get_combined_score(base_word, rhyme_word, phonetic_score)
            scored_rhymes.append((rhyme_word, combined_score))
        
        return sorted(scored_rhymes, key=lambda x: x[1], reverse=True)