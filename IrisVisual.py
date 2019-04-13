# this code will show how simple it is to plot a decision tree (or to visualize it :))
# 1st: Install GraphViz, I have already done downloading 
# setting system 'PATH' on windows in case it (the graphviz) does not recognize it
# 2st: Coding:

from sklearn.tree import export_graphviz
from sklearn import datasets as ds

iris = ds.load_iris ()
# iris loaded :)
# i skip the step of spliting due to laziness, but it is every important

# iris.data, iris.target  (the label)
from sklearn.tree import DecisionTreeClassifier as dtc
model = dtc(min_samples_leaf=10,max_depth=10)
model.fit (iris.data, iris.target)

# now visualizing it (the 'model')
from io import StringIO
file = StringIO ()
export_graphviz (model, out_file=file)
import pydotplus 
graph = pydotplus.graph_from_dot_data (file.getvalue())
graph.write_png ('TEST.PNG')

# now we will try
# it plots in TEST.PNG
# now we plot to screen after saving to file
import matplotlib.image as mimg
img = mimg.imread ('TEST.PNG')
import matplotlib.pyplot as plt
plt.imshow (img)
plt.show ()
