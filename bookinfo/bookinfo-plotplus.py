import matplotlib.pyplot as plt
import numpy as np
import matplotlib

matplotlib.use('Agg')  # Use non-interactive backend

font_size = 13

# Only keep SIZE 0 data
data_basic = {
    'bookinfo': {
        'avg_latency': 315.27,
        'ci_latency': 6.31,
        'avg_requests': 29.93,
        'ci_requests': 0.04
    }
}

custom_epbf = {
    'bookinfo': {
        'avg_latency': 318.41,
        'ci_latency': 3.59,
        'avg_requests': 29.96,
        'ci_requests': 0.03
    }
}

packetProcess = {
    'bookinfo': {
        'avg_latency': 314.61,
        'ci_latency': 8.40,
        'avg_requests': 29.94,
        'ci_requests': 0.03
    }
}

current_color_list = [
    "#ffce94", "#9cd69c", "#c6b5d6", "#b5e7ff", "#a5a5a5", 
    "#4472c4", "#ed7d31", "#a5a5a5", "#ffc000", "#70ad47", 
    "#5b9bd5", "#ff6b6b", "#4ecdc4", "#45b7d1", "#e74c3c", 
    "#2ecc71", "#e67e22", "#3498db", "#e74c3c", "#27ae60"
]

def plot_latency(data1, data2, data3):
    line_width = 2
    bar_width = 0.2
    gap_width = 0.2
    
    # Calculate x positions for single group
    x1 = np.array([0])  # Only one position needed
    x2 = x1 + bar_width 
    x3 = x2 + bar_width

    plt.figure(figsize=(8, 6))

    plt.bar(x1, [data1[label]['avg_latency'] for label in data1.keys()], 
            bar_width, yerr=[data1[label]['ci_latency'] for label in data1.keys()],
            color=current_color_list[11], capsize=5, edgecolor='black', 
            label='Original Cilium', alpha=0.7)
            
    plt.bar(x2, [data2[label]['avg_latency'] for label in data2.keys()],
            bar_width, yerr=[data2[label]['ci_latency'] for label in data2.keys()],
            color=current_color_list[19], capsize=5, edgecolor='black', 
            label='Custom BPF', alpha=0.7)
            
    plt.bar(x3, [data3[label]['avg_latency'] for label in data3.keys()],
            bar_width, yerr=[data3[label]['ci_latency'] for label in data3.keys()],
            color=current_color_list[2], capsize=5, edgecolor='black', 
            label='Packet Process BPF', alpha=0.7)

    plt.xlabel('Implementation Type', fontsize=font_size)
    plt.ylabel('Latency [ms]', fontsize=font_size)
    
    # Remove x-axis labels since we only have one group
    plt.xticks([])
    plt.yticks(fontsize=font_size)

    plt.legend(fontsize=font_size, loc='upper center', bbox_to_anchor=(0.5, 1.15),
              ncol=3, handlelength=1.2, handleheight=0.8, columnspacing=0.2, labelspacing=0.1)

    plt.tight_layout()
    plt.savefig('latency_plot.png')
    plt.close()

def plot_requests(data1, data2, data3):
    line_width = 2
    bar_width = 0.2
    gap_width = 0.2
    
    x1 = np.array([0])
    x2 = x1 + bar_width
    x3 = x2 + bar_width

    plt.figure(figsize=(8, 6))

    plt.bar(x1, [data1[label]['avg_requests'] for label in data1.keys()],
            bar_width, yerr=[data1[label]['ci_requests'] for label in data1.keys()],
            color=current_color_list[11], capsize=5, edgecolor='black',
            label='Original Cilium', alpha=0.7)
            
    plt.bar(x2, [data2[label]['avg_requests'] for label in data2.keys()],
            bar_width, yerr=[data2[label]['ci_requests'] for label in data2.keys()],
            color=current_color_list[19], capsize=5, edgecolor='black',
            label='Custom BPF', alpha=0.7)
            
    plt.bar(x3, [data3[label]['avg_requests'] for label in data3.keys()],
            bar_width, yerr=[data3[label]['ci_requests'] for label in data3.keys()],
            color=current_color_list[2], capsize=5, edgecolor='black',
            label='Packet Process BPF', alpha=0.7)

    plt.xlabel('Implementation Type', fontsize=font_size)
    plt.ylabel('Requests [1/Secs]', fontsize=font_size)
    
    plt.xticks([])
    plt.yticks(fontsize=font_size)

    plt.legend(fontsize=font_size, loc='upper center', bbox_to_anchor=(0.5, 1.15),
              ncol=3, handlelength=1.2, handleheight=0.8, columnspacing=0.2, labelspacing=0.1)

    plt.tight_layout()
    plt.savefig('requests_plot.png')
    plt.close()

if __name__ == "__main__":
    plot_latency(data_basic, custom_epbf, packetProcess)
    plot_requests(data_basic, custom_epbf, packetProcess)