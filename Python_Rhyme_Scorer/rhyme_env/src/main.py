import sys
from rhyme_quality_scorer import PyTorchRhymeScorer
from rhyme_utils import process_rhyme_input, save_scores_for_cpp

def main():
    if len(sys.argv) != 2:
        print("Usage: python main.py input_file")
        print("Input file should be JSON with format:")
        print('{"base_word": "example", "candidates": [["word1", 0.9], ["word2", 0.8]]}')
        return 1
    
    try:
        print("Initializing PyTorch rhyme scorer...")
        scorer = PyTorchRhymeScorer()
        
        print("Processing input file...")
        base_word, rhyme_candidates = process_rhyme_input(sys.argv[1])
        
        print("Ranking rhymes...")
        ranked_rhymes = scorer.rank_rhymes(base_word, rhyme_candidates)
        
        print("Saving scores...")
        save_scores_for_cpp(ranked_rhymes)
        
        print(f"\nRanked rhymes for '{base_word}':")
        print("-" * 40)
        for word, score in ranked_rhymes:
            print(f"{word}: {score:.4f}")
            
    except Exception as e:
        print(f"Error: {str(e)}")
        return 1
    
    return 0

if __name__ == "__main__":
    sys.exit(main())