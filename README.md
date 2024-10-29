# Rhyme Generation and Analysis System

A comprehensive system for finding perfect and slant rhymes, analyzing rhyme patterns, and generating rhyming word pairs for machine learning applications. This project combines C++ for efficient rhyme lookup and Python for advanced pattern analysis and machine learning data preparation.

## Project Components

### 1. RhymeFinder (C++)

- Fast lookup of perfect rhymes using phonetic patterns
- Command-line interface for interactive rhyme searches
- Based on CMU Phonetic Dictionary data
- Hash table implementation for efficient pattern matching

### 2. Python_rhyme_pair_gen

- Generates training data for machine learning models
- Creates balanced datasets of rhyming and non-rhyming word pairs
- Includes validation set generation
- Supports slant rhyme detection

### 3. Rhyme_data_generator

- Tools for processing and formatting CMU Dictionary data
- Generates rhyme pattern maps
- Supports different output formats (TXT and C++)

## Getting Started

### Prerequisites

- C++ compiler with C++11 support
- Python 3.7+

### Installation

1. Clone the repository:

2. Set up the Python environment:

```bash
cd Python_rhyme_pair_gen
python -m venv rhyme_env
source rhyme_env/bin/activate
pip install -r requirements.txt
```

3. Build the C++ component:

## Usage

### Perfect Rhyme's Finder (C++)

```bash
./RhymeFinder
# Enter words to find perfect rhymes
```

### Generate Training Data (Python)

```bash
cd Python_rhyme_pair_gen
python rhyme_pair_generator.py
```

### Slant Rhyme Detection

```python
from slant_rhyme_finder import SlantRhymeFinder
finder = SlantRhymeFinder()
finder.load_cmu_dict('path_to_cmudict.txt')
slant_rhymes = finder.find_slant_rhymes('queen')
```

## Data Files

- `rhyme_map.txt`: Mapping of phonetic patterns to rhyming words
- `rhyme_train.txt`: Training pairs for machine learning
- `rhyme_val.txt`: Validation pairs
- `rhyme_negative.txt`: Non-rhyming word pairs
