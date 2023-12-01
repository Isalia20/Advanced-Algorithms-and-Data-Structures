import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd


def parse_heights_txt(file_text: str):
    with open(file_text, 'r') as f:
        all_skip_list_heights = []
        skip_list_heights = []
        for line in f:
            line = line.replace("\n", "")
            if line == "---------------------------------": # End of skip list
                all_skip_list_heights.append(skip_list_heights)
                skip_list_heights = []
                continue
            skip_list_heights.append(int(line))
    return all_skip_list_heights

def visualize_height_mean_and_std(data, mean_values, std_values):
    # Create an array of feature names for labeling purposes
    features = ['Level ' + str(13 - i) for i in range(data.shape[1])]

    # Create a dataframe for better visualization
    df = pd.DataFrame({'Levels': features, 'Mean': mean_values, 'Std': std_values})

    # Plotting
    _, ax1 = plt.subplots(figsize=(20, 10))

    # Bar plot for mean
    bar = sns.barplot(x='Levels', y='Mean', data=df, ax=ax1, color='skyblue', ci=None)

    # Annotate numbers on each bar
    for p in bar.patches:
        ax1.text(p.get_x() + p.get_width() / 2., p.get_height(), '{0:.2f}'.format(p.get_height()), 
                 fontsize=12, color='black', ha='center', va='bottom')


    # Line plot for std on the second y-axis
    ax2 = ax1.twinx()
    sns.lineplot(x='Levels', y='Std', data=df, ax=ax2, color='r', sort=False)
    
    ax1.set_title("Random SkipList Level Node count mean and standard deviation for 10,000 element array")
    
    for i, y in enumerate(df['Std']):
        ax2.text(i, y, '{0:.2f}'.format(y), fontsize=12, color='red', ha='center', va='top')

    # Show the graph
    plt.savefig("RandomSkipListLevelNodeCount.png")

if __name__ == "__main__":
    skip_list_heights = parse_heights_txt("statistical_comparisons/height results.txt")
    skip_list_heights = np.array(skip_list_heights)
    mean_values = np.mean(skip_list_heights, axis=0)
    std_values = np.std(skip_list_heights, axis=0)
    visualize_height_mean_and_std(skip_list_heights, mean_values, std_values)
