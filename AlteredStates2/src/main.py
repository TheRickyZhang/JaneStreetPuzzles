import os
import time
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

import tensorflow as tf
tf.get_logger().setLevel('ERROR')
tf.autograph.set_verbosity(3)

import logging
logging.getLogger('tensorflow').setLevel(logging.ERROR)
logging.basicConfig(level=logging.ERROR)

from environment import GridEnvironment
from dqn_agent import DQNAgent
import numpy as np

if __name__ == "__main__":

    # Best grid so far: TONAWGEFSIVILONNARYCCHTKA
    # env = GridEnvironment()
    # env.grid = ['T', 'O', 'N', 'A', 'W', 'G', 'E', 'F', 'S', 'I', 'V', 'I', 'L', 'O', 'N', 'N', 'A', 'R', 'Y', 'C',
    #             'C', 'H', 'T', 'K', 'A']
    # print(env.calculate_reward())

    env = GridEnvironment()
    state_size = 25
    action_size = 26 * 25  # Place one of 26 letters in one of 25 spots
    agent = DQNAgent(state_size, action_size)
    episodes = 1000   # episode = clear start and end for training session, to reshape the neurons
    batch_size = 32  # At each step, sample 32 experiences from the replay memory to train the model

    iteration_count = 0
    print_interval = 10

    start_time = time.time()
    for e in range(episodes):
        print(f"Episode {e}")
        state = env.reset()  # Initialize the grid at the beginning of each episode

        # Reshape state to fit neural network input
        state = np.reshape(state, [1, state_size])
        for time_step in range(500):
            # Select action based on current state (uses e-greedy strategy)
            action = agent.act(state)
            # Ensure the action is valid
            if action >= 26 * 25 or action < 0:
                continue

            # Adjust action for 0-indexing
            grid_index, letter_num = divmod(action, 26)

            # Take a step in the environment using the selected action
            next_state, reward, done = env.step((grid_index, letter_num))

            next_state = np.reshape(next_state, [1, state_size])

            # Store experience and update state
            agent.remember(state, action, reward, next_state, done)
            state = next_state

            if done:
                agent.update_target_model()
                print(f"episode: {e}/{episodes}, score: {env.current_reward}, e: {agent.epsilon}, grid: {''.join(env.decode_grid(state))}")
                break
            if len(agent.memory) > batch_size:
                agent.replay(batch_size)

            iteration_count += 1
            print(f"Iteration {iteration_count}")

            if iteration_count % print_interval == 0:
                current_grid = env.decode_grid(state)
                print(f"Current grid at episode {e}, iteration {iteration_count}:\n" + "\n".join(["".join(current_grid[i:i+5]) for i in range(0, 25, 5)]))

