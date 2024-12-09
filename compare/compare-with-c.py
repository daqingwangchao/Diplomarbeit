import matplotlib.pyplot as plt
import numpy as np
import matplotlib

matplotlib.use('Agg')  # Use non-interactive backend

font_size = 13

# Extended data structure with three datasets
data_basic = {
    '(c1, t1)': {
        'avg_latency': 9.26,
        'ci_latency': 1.25,
        'avg_requests': 200.68,
        'ci_requests': 10.28
    },
    '(c2,t2)': {
        'avg_latency': 11.25,
        'ci_latency': 1.06,
        'avg_requests': 353.58,
        'ci_requests': 30.23
    },
    '(c4,t3)': {
        'avg_latency': 9.91,
        'ci_latency': 2.23,
        'avg_requests': 361.44,
        'ci_requests': 42.64
    },
    '(c8,t6)': {
        'avg_latency': 10.87,
        'ci_latency': 1.82,
        'avg_requests': 434.83,
        'ci_requests': 12.02
    },
    '(c16,t10)': {
        'avg_latency': 14.78,
        'ci_latency': 3.43,
        'avg_requests': 551.32,
        'ci_requests': 30.02
    }
}

custom_epbf = {
    '(c1,t1)': {
        'avg_latency': 49.06,
        'ci_latency': 18.30,
        'avg_requests': 39.28,
        'ci_requests': 3.03
    },
    '(c2,t2)': {
        'avg_latency': 20.24,
        'ci_latency': 5.62,
        'avg_requests': 71.71,
        'ci_requests': 15.02
    },
    '(c4,t3)': {
        'avg_latency': 20.32,
        'ci_latency': 2.30,
        'avg_requests': 86.28,
        'ci_requests': 22.02
    },
    '(c8,t6)': {
        'avg_latency': 30.9,
        'ci_latency': 2.53,
        'avg_requests': 115.58,
        'ci_requests': 24.02
    },
    '(c16,t10)': {
        'avg_latency': 40.79,
        'ci_latency': 13.72,
        'avg_requests': 137.48,
        'ci_requests': 12.02
    }
}

# New third dataset
packetProcess = {
    '(c1,t1)': {
        'avg_latency': 40.86,
        'ci_latency': 5.23,
        'avg_requests': 34.89,
        'ci_requests': 3.85
    },
    '(c2,t2)': {
        'avg_latency': 59.91,
        'ci_latency': 20.61,
        'avg_requests': 39.94,
        'ci_requests': 3.12
    },
    '(c4,t3)': {
        'avg_latency': 57.56,
        'ci_latency': 1.31,
        'avg_requests': 35.71,
        'ci_requests': 13.02
    },
    '(c8,t6)': {
        'avg_latency': 122.78,
        'ci_latency': 8.40,
        'avg_requests': 26.09,
        'ci_requests': 9.02
    },
    '(c16,t10)': {
        'avg_latency': 201.15,
        'ci_latency': 10.72,
        'avg_requests': 21.97,
        'ci_requests': 3.02
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
           label='pggymetric (Left)', alpha=0.7)
           
   plt.bar(x2, [data2[label]['avg_latency'] for label in data2.keys()],
           bar_width, yerr=[data2[label]['ci_latency'] for label in data2.keys()],
           color=current_color_list[19], capsize=5, edgecolor='black', 
           label='Sock Shop (Middle)', alpha=0.7)
           
   plt.bar(x3, [data3[label]['avg_latency'] for label in data3.keys()],
           bar_width, yerr=[data3[label]['ci_latency'] for label in data3.keys()],
           color=current_color_list[2], capsize=5, edgecolor='black', 
           label='Bookinfo (Right)', alpha=0.7)

   plt.xlabel('Connection number', fontsize=font_size)
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
           label='pggymetric (Left)', alpha=0.7)
           
   plt.bar(x2, [data2[label]['avg_requests'] for label in data2.keys()],
           bar_width, yerr=[data2[label]['ci_requests'] for label in data2.keys()],
           color=current_color_list[19], capsize=5, edgecolor='black',
           label='Sock Shop (Middle)', alpha=0.7)
           
   plt.bar(x3, [data3[label]['avg_requests'] for label in data3.keys()],
           bar_width, yerr=[data3[label]['ci_requests'] for label in data3.keys()],
           color=current_color_list[2], capsize=5, edgecolor='black',
           label='Bookinfo (Right)', alpha=0.7)

   plt.xlabel('Connection number', fontsize=font_size)
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