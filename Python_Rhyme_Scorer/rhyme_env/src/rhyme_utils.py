import json
import csv

def process_rhyme_input(filepath):
    """Process input file containing base word and rhyme candidates"""
    try:
        with open(filepath, 'r') as f:
            data = json.load(f)
            base_word = data.get('base_word', '')
            candidates = [(word, score) for word, score in data.get('candidates', [])]
            return base_word, candidates
    except json.JSONDecodeError:
        print("Error: Input file must be valid JSON")
        raise
    except KeyError:
        print("Error: Input file must contain 'base_word' and 'candidates' fields")
        raise
    except Exception as e:
        print(f"Error processing input file: {str(e)}")
        raise

def save_scores_for_cpp(scored_rhymes, output_file='rhyme_scores.csv'):
    """Save scored rhymes to CSV file for C++ processing"""
    try:
        with open(output_file, 'w', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(['word', 'score'])  # header
            for word, score in scored_rhymes:
                writer.writerow([word, f"{score:.6f}"])
        print(f"Scores saved to {output_file}")
    except Exception as e:
        print(f"Error saving scores: {str(e)}")
        raise