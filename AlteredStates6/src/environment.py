import json
import os
import numpy as np
from trie import Trie

class GridEnvironment:
    def __init__(self):
        self.grid = [''] * 25
        base_path = os.path.dirname(os.path.abspath(__file__))
        file_path = os.path.join(base_path, '../data/state_populations.json')
        with open(file_path) as f:
            self.state_population = json.load(f)
        self.valid_states = list(self.state_population.keys())
        self.letter_to_num = {chr(i + 65): i + 1 for i in range(26)}  # A=1, B=2, ..., Z=26
        self.letter_to_num[''] = 0  # Empty cells
        self.num_to_letter = {v: k for k, v in self.letter_to_num.items()}  # Reverse mapping

        # Build the trie with valid states
        self.trie = Trie()
        for state in self.valid_states:
            self.trie.insert(state)

    def reset(self):
        self.grid = [''] * 25
        return self.encode_grid(self.grid)

    def step(self, action):
        idx, letter_num = action
        letter = self.num_to_letter[letter_num]
        self.grid[idx] = letter
        reward = self.calculate_reward()
        done = self.is_done()
        return self.encode_grid(self.grid), reward, done

    def calculate_reward(self):
        reward = 0
        grid_str = ''.join(self.grid)
        for i in range(len(grid_str) - 1):  # Check all substrings of the grid for valid states
            for j in range(i + 1, len(grid_str) + 1):
                substring = grid_str[i:j]
                if self.trie.search_with_one_letter_alteration(substring):
                    reward += self.state_population.get(substring, 0)
        return reward

    def is_valid_state(self, state):
        return self.trie.search_with_one_letter_alteration(state)

    def is_done(self):
        return all(cell != '' for cell in self.grid)

    def encode_grid(self, grid):
        return np.array([self.letter_to_num.get(cell, 0) for cell in grid]).reshape(1, -1)

    def decode_grid(self, encoded_grid):
        return [self.num_to_letter.get(num, '') for num in encoded_grid[0]]
