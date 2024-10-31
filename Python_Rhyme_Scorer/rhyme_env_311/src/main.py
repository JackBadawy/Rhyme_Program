import sys
from rhyme_quality_scorer import PyTorchRhymeScorer
from rhyme_utils import process_rhyme_input, save_scores_for_cpp

def main():
    scorer = PyTorchRhymeScorer()
    scorer.train_on_gutenberg(num_poems=2, epochs=3)  

    test_pairs = [
        ("light", "bright", 0.9),
        ("night", "sight", 0.9),
        ("blue", "true", 0.9),
        ("day", "way", 0.9)
    ]

    for word1, word2, _ in test_pairs:
        score = scorer.get_combined_score(word1, word2, 0.9)
        print(f"{word1} - {word2}: {score:.4f}")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())