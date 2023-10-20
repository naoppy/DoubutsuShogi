import pickle

if __name__ == "__main__":
    with open("visited_dict.pkl", "rb") as f:
        visited_dict = pickle.load(f)
    print(len(visited_dict))