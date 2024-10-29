import sys
from rhyme_quality_scorer import SimpleRhymeScorer, process_rhyme_input, save_scores_for_cpp

def main():
    if len(sys.argv) != 2:
        print("Usage: python main.py input_file")
        return 1
    
    try:
        scorer = SimpleRhymeScorer()
        base_word, rhyme_candidates = process_rhyme_input(sys.argv[1])
        ranked_rhymes = scorer.rank_rhymes(base_word, rhyme_candidates)
        save_scores_for_cpp(ranked_rhymes)
        
        print(f"\nRanked rhymes for '{base_word}':")
        for word, score in ranked_rhymes:
            print(f"{word}: {score:.4f}")
            
    except Exception as e:
        print(f"Error: {str(e)}")
        return 1
    
    return 0

if __name__ == "__main__":
    sys.exit(main())