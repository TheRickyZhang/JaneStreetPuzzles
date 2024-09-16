import numpy as np
import random
from collections import deque
import tensorflow as tf
from tensorflow.keras import models, layers, optimizers

class DQNAgent:
    def __init__(self, state_size, action_size):
        self.state_size = state_size  # Size of the state space (25 for a 5x5 grid)
        self.action_size = action_size  # Size of the action space (26 letters * 25 positions)
        self.memory = deque(maxlen=2000)  # Experience replay memory
        self.gamma = 0.95  # Discount rate for future rewards
        self.epsilon = 1.0  # Exploration rate (initially fully exploratory)
        self.epsilon_min = 0.01  # Minimum exploration rate
        self.epsilon_decay = 0.995  # Decay rate for exploration probability
        self.learning_rate = 0.001  # Learning rate for the optimizer
        self.model = self._build_model()  # Build the Q-network model
        self.target_model = self._build_model()  # Build the target Q-network model
        self.update_target_model()  # Initialize target model with the same weights as the Q-network

    def _build_model(self):
        # Build a neural network for approximating the Q-values
        model = models.Sequential()
        model.add(layers.Input(shape=(self.state_size,)))  # Input layer
        model.add(layers.Dense(24, activation='relu'))  # First hidden layer
        model.add(layers.Dense(24, activation='relu'))  # Second hidden layer
        model.add(layers.Dense(self.action_size, activation='linear'))  # Output layer
        model.compile(loss='mse', optimizer=optimizers.Adam(learning_rate=self.learning_rate))  # Compile the model
        return model

    def update_target_model(self):
        # Update the target network to have the same weights as the Q-network
        self.target_model.set_weights(self.model.get_weights())

    def remember(self, state, action, reward, next_state, done):
        # Store the experience in memory
        self.memory.append((state, action, reward, next_state, done))

    def act(self, state):
        # Choose an action based on the current state (exploration vs exploitation)
        if np.random.rand() <= self.epsilon:
            return random.randrange(self.action_size)  # Explore: choose a random action
        act_values = self.model.predict(state, verbose=0)  # Exploit: choose the best action based on Q-values
        return np.argmax(act_values[0])

    def replay(self, batch_size):
        # Train the Q-network using experience replay
        minibatch = random.sample(self.memory, batch_size)  # Sample a minibatch of experiences
        for state, action, reward, next_state, done in minibatch:
            target = reward
            if not done:
                # Compute the target Q-value using the target network
                next_state_prediction = self.target_model.predict(next_state, verbose=0)[0]
                target = (reward + self.gamma * np.amax(next_state_prediction))
            target_f = self.model.predict(state, verbose=0)  # Predict Q-values for the current state
            target_f[0][action] = target  # Update the Q-value for the taken action
            self.model.fit(state, target_f, epochs=1, verbose=0)  # Train the Q-network
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay  # Decay the exploration rate
