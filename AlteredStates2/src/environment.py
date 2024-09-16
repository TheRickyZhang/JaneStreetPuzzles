import json
import os
import numpy as np
from collections import Counter
from trie import Trie

class GridEnvironment:
    # Class-level constants
    BASE_PATH = os.path.dirname(os.path.abspath(__file__))
    FILE_PATH = os.path.join(BASE_PATH, '../data/state_populations.json')
    with open(FILE_PATH) as f:
        STATE_POPULATION = json.load(f)
    VALID_STATES = list(STATE_POPULATION.keys())
    LETTER_TO_NUM = {chr(i + 65): i for i in range(26)}  # A=0, B=1, ..., Z=25
    LETTER_TO_NUM[''] = -1  # Empty cells
    NUM_TO_LETTER = {v: k for k, v in LETTER_TO_NUM.items() if k != ''}  # Reverse mapping
    SCORE_THRESHOLD = 150000000  # Score threshold set to 150 million
    MAX_ITERATIONS = 500  # Maximum iterations per episode

    # Build the trie with valid states
    TRIE = Trie()
    for state in VALID_STATES:
        TRIE.insert(state)

    def __init__(self):
        self.grid = [''] * 25
        self.current_reward = 0  # Track current reward for the current grid
        self.iteration_count = 0  # Track the number of iterations in the current episode

    def reset(self):
        # Initialize the grid with random letters
        self.current_reward = 0  # Reset current reward
        self.iteration_count = 0  # Reset iteration count
        print("reset called")
        self.grid = [chr(np.random.randint(65, 91)) for _ in range(25)]  # A-Z
        return self.encode_grid(self.grid)

    def step(self, action):
        idx, letter_num = action
        if 0 <= letter_num <= 25:
            letter = self.NUM_TO_LETTER[letter_num]
            self.grid[idx] = letter
        else:
            print(f"Invalid letter_num: {letter_num}")

        reward = self.calculate_reward()
        if reward > 10000000:
            print(reward)
        self.current_reward = reward  # Update current reward
        self.iteration_count += 1  # Increment iteration count
        done = self.is_done()
        # print("Grid after step:", self.grid)  # Debug print for grid state
        return self.encode_grid(self.grid), reward, done

    def calculate_reward(self):
        reward = 0
        directions = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]  # King's moves

        def dfs(x, y, state, index):
            if index == len(state):
                return True
            if not (0 <= x < 5 and 0 <= y < 5) or self.grid[x * 5 + y] != state[index]:
                return False
            for dx, dy in directions:
                if dfs(x + dx, y + dy, state, index + 1):
                    return True
            return False

        def generate_variations(state):
            variations = set()
            for i in range(len(state)):
                for letter in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
                    if state[i] != letter:
                        variation = state[:i] + letter + state[i + 1:]
                        variations.add(variation)
            return variations

        found_states = set()
        for state in self.VALID_STATES:
            variations = generate_variations(state)
            variations.add(state)  # Include the original state
            for variation in variations:
                for i in range(5):
                    for j in range(5):
                        if dfs(i, j, variation, 0):
                            found_states.add(state)
                            break

        for state in found_states:
            print(state)
            reward += self.STATE_POPULATION[state]

        return reward

    def is_valid_state(self, state):
        return self.TRIE.search_with_one_letter_alteration(state)

    def is_done(self):
        # Done if the current reward exceeds the score threshold or max iterations are reached
        return self.current_reward >= self.SCORE_THRESHOLD or self.iteration_count >= self.MAX_ITERATIONS

    def encode_grid(self, grid):
        return np.array([self.LETTER_TO_NUM.get(cell, 0) for cell in grid]).reshape(1, -1)

    def decode_grid(self, encoded_grid):
        return [self.NUM_TO_LETTER.get(num, '') for num in encoded_grid[0]]
