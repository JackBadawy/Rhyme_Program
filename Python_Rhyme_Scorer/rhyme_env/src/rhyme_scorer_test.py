import unittest
from rhyme_quality_scorer import SimpleRhymeScorer

def test_rhyme_scorer():
    """Simple test function to demonstrate the rhyme scorer"""
    print("Initializing rhyme scorer...")
    scorer = SimpleRhymeScorer()
    
    test_cases = [
        (
            "light",
            [
                ("bright", 0.9),
                ("flight", 0.9),
                ("site", 0.85),
                ("bite", 0.9),
                ("kite", 0.9)
            ]
        ),
        (
            "blue",
            [
                ("true", 0.9),
                ("flew", 0.9),
                ("zoo", 0.85),
                ("new", 0.9)
            ]
        ),
        (
            "dream",
            [
                ("seem", 0.9),
                ("team", 0.9),
                ("beam", 0.9),
                ("cream", 0.9)
            ]
        )
    ]
    
    for base_word, rhyme_candidates in test_cases:
        print(f"\nTesting rhymes for '{base_word}':")
        print("-" * 40)
        
        ranked_rhymes = scorer.rank_rhymes(base_word, rhyme_candidates)
        
        print("Ranked results (word: score):")
        for word, score in ranked_rhymes:
            print(f"{word}: {score:.4f}")

if __name__ == "__main__":
    test_rhyme_scorer()