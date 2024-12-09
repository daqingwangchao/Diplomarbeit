import matplotlib.pyplot as plt
import numpy as np
import matplotlib

matplotlib.use('Agg')  # Use non-interactive backend

font_size = 13

# Extended data structure with three datasets
data_basic = {
    'SIZE 0': {
        'avg_latency': 5.71,
        'ci_latency': 0.64,
        'avg_requests': 30.34,
        'ci_requests': 0.06
    },
    'SIZE 4000': {
        'avg_latency': 9.76,
        'ci_latency': 0.72,
        'avg_requests': 30.32,
        'ci_requests': 0.02
    }
}

custom_epbf = {
    'SIZE 0': {
        'avg_latency': 5.49,
        'ci_latency': 0.47,
        'avg_requests': 30.31,
        'ci_requests': 0.01
    },
    'SIZE 4000': {
        'avg_latency': 9.12,
        'ci_latency': 0.99,
        'avg_requests': 30.32,
        'ci_requests': 0.03
    }
}

# New third dataset
packetProcess = {
    'SIZE 0': {
        'avg_latency': 5.06,  
        'ci_latency': 0.41,
        'avg_requests': 30.33,
        'ci_requests': 0.03
    },
    'SIZE 4000': {
        'avg_latency': 9.51,
        'ci_latency': 0.46,
        'avg_requests': 30.31,
        'ci_requests': 0.02
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
   bar_width = 0.2  # 保持一致的bar宽度
   gap_width = 0.2  # 添加明确的间隔宽度
   
   # 计算x轴位置
   x1 = np.arange(len(data1.keys())) * (3 * bar_width + gap_width)  # 每组之间增加gap_width的间隔
   x2 = x1 + bar_width 
   x3 = x2 + bar_width

   plt.figure(figsize=(12, 6))

   # 使用新的x轴位置绘制条形图
   plt.bar(x1, [data1[label]['avg_latency'] for label in data1.keys()], 
           bar_width, yerr=[data1[label]['ci_latency'] for label in data1.keys()],
           color=current_color_list[11], capsize=5, edgecolor='black', 
           label='Using original cilium (Left)', alpha=0.7)
           
   plt.bar(x2, [data2[label]['avg_latency'] for label in data2.keys()],
           bar_width, yerr=[data2[label]['ci_latency'] for label in data2.keys()],
           color=current_color_list[19], capsize=5, edgecolor='black', 
           label='Adding customize BPF (Middle)', alpha=0.7)
           
   plt.bar(x3, [data3[label]['avg_latency'] for label in data3.keys()],
           bar_width, yerr=[data3[label]['ci_latency'] for label in data3.keys()],
           color=current_color_list[2], capsize=5, edgecolor='black', 
           label='packetProcess BPF (Right)', alpha=0.7)

   plt.xlabel('Message Size [Bytes]', fontsize=font_size)
   plt.ylabel('Latency [ms]', fontsize=font_size)
   
   # 调整x轴刻度位置和标签
   plt.xticks(x2, data1.keys(), fontsize=font_size)  # 使用中间条的位置作为刻度位置
   plt.yticks(fontsize=font_size)

   plt.legend(fontsize=font_size, loc='upper center', bbox_to_anchor=(0.5, 1.15),
             ncol=3, handlelength=1.2, handleheight=0.8, columnspacing=0.2, labelspacing=0.1)

   # 添加分隔线在每组之间
   for i in range(len(data1.keys())-1):
       plt.axvline(x=x3[i] + (gap_width + bar_width)/2, color='black', linewidth=line_width)

   plt.tight_layout()
   plt.savefig('latency_plot.png')
   plt.close()

def plot_requests(data1, data2, data3):
   # 使用与plot_latency完全相同的参数
   line_width = 2
   bar_width = 0.2
   gap_width = 0.2
   
   x1 = np.arange(len(data1.keys())) * (3 * bar_width + gap_width)
   x2 = x1 + bar_width
   x3 = x2 + bar_width

   plt.figure(figsize=(12, 6))

   plt.bar(x1, [data1[label]['avg_requests'] for label in data1.keys()],
           bar_width, yerr=[data1[label]['ci_requests'] for label in data1.keys()],
           color=current_color_list[11], capsize=5, edgecolor='black',
           label='Using Original cilium (Left)', alpha=0.7)
           
   plt.bar(x2, [data2[label]['avg_requests'] for label in data2.keys()],
           bar_width, yerr=[data2[label]['ci_requests'] for label in data2.keys()],
           color=current_color_list[19], capsize=5, edgecolor='black',
           label='Adding customize BPF (Middle)', alpha=0.7)
           
   plt.bar(x3, [data3[label]['avg_requests'] for label in data3.keys()],
           bar_width, yerr=[data3[label]['ci_requests'] for label in data3.keys()],
           color=current_color_list[2], capsize=5, edgecolor='black',
           label='Optimized BPF (Right)', alpha=0.7)

   plt.xlabel('Message Size [Bytes]', fontsize=font_size)
   plt.ylabel('Requests [1/Secs]', fontsize=font_size)
   
   plt.xticks(x2, data1.keys(), fontsize=font_size)
   plt.yticks(fontsize=font_size)

   plt.legend(fontsize=font_size, loc='upper center', bbox_to_anchor=(0.5, 1.15),
             ncol=3, handlelength=1.2, handleheight=0.8, columnspacing=0.2, labelspacing=0.1)

   # 添加分隔线在每组之间
   for i in range(len(data1.keys())-1):
       plt.axvline(x=x3[i] + (gap_width + bar_width)/2, color='black', linewidth=line_width)

   plt.tight_layout()
   plt.savefig('requests_plot.png')
   plt.close()

if __name__ == "__main__":
    plot_latency(data_basic, custom_epbf, packetProcess)  # Plot Latency
    plot_requests(data_basic, custom_epbf, packetProcess)  # Plot Requests