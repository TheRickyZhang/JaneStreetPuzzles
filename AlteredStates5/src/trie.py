class TrieNode:
    def __init__(self):
        self.children = {}
        self.is_end_of_word = False

class Trie:
    def __init__(self):
        self.root = TrieNode()

    def insert(self, word):
        node = self.root
        for char in word:
            if char not in node.children:
                node.children[char] = TrieNode()
            node = node.children[char]
        node.is_end_of_word = True

    def search_with_one_letter_alteration(self, word):
        def dfs(node, pos, changed):
            if pos == len(word):
                return node.is_end_of_word and changed
            if word[pos] in node.children:
                if dfs(node.children[word[pos]], pos + 1, changed):
                    return True
            if not changed:
                for char in node.children:
                    if char != word[pos] and dfs(node.children[char], pos + 1, True):
                        return True
            return False

        return dfs(self.root, 0, False)
