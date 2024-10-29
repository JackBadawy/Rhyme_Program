import numpy as np
import gensim.downloader as api

class SimpleRhymeScorer:
    def __init__(self):
        """Initialize the scorer with GloVe embeddings"""
        print("Loading word embeddings...")
        self.word_vectors = api.load('glove-twitter-25')  
        print("Word embeddings loaded")
        self.similarity_cache = {}
        
    def get_combined_score(self, base_word, rhyme_word, phonetic_score):
        """
        Calculate a combined score based on semantic similarity and phonetic score
        
        Args:
            base_word (str): The original word
            rhyme_word (str): The rhyming word
            phonetic_score (float): Score from the phonetic rhyming algorithm (0-1)
            
        Returns:
            float: Combined score between 0 and 1
        """
        try:
            cache_key = tuple(sorted([base_word.lower(), rhyme_word.lower()]))
            if cache_key in self.similarity_cache:
                semantic_score = self.similarity_cache[cache_key]
            else:
                vec1 = self.word_vectors[base_word.lower()]
                vec2 = self.word_vectors[rhyme_word.lower()]
                semantic_score = np.dot(vec1, vec2) / (np.linalg.norm(vec1) * np.linalg.norm(vec2))
                self.similarity_cache[cache_key] = semantic_score
            
            semantic_score = (semantic_score + 1) / 2
            combined_score = (0.7 * phonetic_score) + (0.3 * semantic_score)
            return combined_score
            
        except (KeyError, ValueError):
            return phonetic_score
        
    def rank_rhymes(self, base_word, rhyme_candidates):
        """
        Rank rhyme candidates based on combined phonetic and semantic scores
        
        Args:
            base_word (str): The original word
            rhyme_candidates (list): List of (word, phonetic_score) tuples
            
        Returns:
            list: Sorted list of (word, score) tuples
        """
        scored_rhymes = []
        
        for rhyme_word, phonetic_score in rhyme_candidates:
            combined_score = self.get_combined_score(base_word, rhyme_word, phonetic_score)
            scored_rhymes.append((rhyme_word, combined_score))
        
        return sorted(scored_rhymes, key=lambda x: x[1], reverse=True)

def process_rhyme_input(input_file):
    """
    Process input file from C++ program
    
    Args:
        input_file (str): Path to input file containing base word and rhyme candidates
    """
    with open(input_file, 'r') as f:
        lines = f.readlines()
    
    base_word = lines[0].strip()
    rhyme_candidates = [(word.strip(), 0.9) for word in lines[1:]]
    return base_word, rhyme_candidates

def save_scores_for_cpp(scored_rhymes, output_file='rhyme_scores.txt'):
    """Save the scored rhymes in a format easily readable by C++"""
    with open(output_file, 'w') as f:
        for word, score in scored_rhymes:
            f.write(f"{word}\t{score:.4f}\n")