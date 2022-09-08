import plotly.express as px
import plotly
import pathlib
import pandas

with open(pathlib.Path.cwd() / "magMag_Temp_2.000000_Lattice_100x100.csv") as infile:
    data = pandas.read_csv(infile, header=0)

# print(data)

fig = px.scatter(data, x = "sweep", y = "m", title = "Ave Mag per Spin 100x100 Lattice T = 2.0")
plotly.io.write_image(fig, "magT2L100x100.pdf", format="pdf")
# fig.show()

his = px.histogram(data, x = "m")
plotly.io.write_image(his, "Hist_magT2L100x100.pdf", format="pdf")
# his.show()