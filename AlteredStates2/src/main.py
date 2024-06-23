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
    env = GridEnvironment()
    state_size = 25
    action_size = 26 * 25
    agent = DQNAgent(state_size, action_size)
    episodes = 1000
    batch_size = 32

    iteration_count = 0
    print_interval = 5000

    start_time = time.time()
    for e in range(episodes):
        print(f"Episode {e}")
        state = env.reset()
        state = np.reshape(state, [1, state_size])
        for time_step in range(500):
            action = agent.act(state)
            next_state, reward, done = env.step(divmod(action, 26))
            reward = reward if not done else -10
            next_state = np.reshape(next_state, [1, state_size])
            agent.remember(state, action, reward, next_state, done)
            state = next_state
            if done:
                agent.update_target_model()
                print(f"episode: {e}/{episodes}, score: {reward}, e: {agent.epsilon}")
                break
            if len(agent.memory) > batch_size:
                agent.replay(batch_size)

            iteration_count += 1
            print(f"Iteration {iteration_count}")

            if iteration_count == 100:
                elapsed_time = time.time() - start_time
                estimated_time_5000 = elapsed_time * 50  # Estimate time for 5000 iterations
                print(f"Time for 100 iterations: {elapsed_time:.2f} seconds")
                print(f"Estimated time for 5000 iterations: {estimated_time_5000:.2f} seconds")

            if iteration_count % print_interval == 0:
                current_grid = env.decode_grid(state[0])
                print(f"Current grid at episode {e}, iteration {iteration_count}: {''.join(current_grid)}")
