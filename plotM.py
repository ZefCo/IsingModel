from genericpath import isfile
import plotly.express as px
import plotly.graph_objects as go
import plotly
import pathlib
import pandas
import re

# with open(pathlib.Path.cwd() / "DataFiles" / "mME_Temp_2.000000_Size_100x100_seed_123456789_sweeps_50000.csv") as infile:
#     data = pandas.read_csv(infile, header=0)

# # # print(data)

# fig = px.scatter(data, x = "sweep", y = "m", title = "")
# # plotly.io.write_image(fig, "magT2L100x100.pdf", format="pdf")
# fig.show()

# his = px.histogram(data, x = "m")
# # plotly.io.write_image(his, "Hist_magT2L100x100.pdf", format="pdf")
# his.show()

# # For the histogram: do it for a bunch of different temps. You should get a gausian at high temp, two seperate clumps
# # at low temp

# Combine Files
csv_folder = pathlib.Path.cwd() / "DataFiles"

# # temps = []
monte_files = pandas.DataFrame()
# eonte_files = pandas.DataFrame()
# scat_files = pandas.DataFrame()
for thing in csv_folder.iterdir():
    # print(type(thing))
    if thing.is_file() and thing.suffix in ".csv":
        # print(thing)
        thingy = re.split("_", str(thing))
        # temp = re.sub(r"\.", ",", thingy[2])
        temp = re.sub(r"000$", "", thingy[2])
        temp = f"{temp}K"
        # print(temp)
        histbins = thingy[4]

        title = f"{temp}_{str(thingy[6])}"

        with open(thing) as thing_file:
            thing_data = pandas.read_csv(thing_file, header = 0)

        m_data = pandas.Series(thing_data["m"].to_list(), name = title, index = thing_data["quarter"])
        # e_data = pandas.Series(thing_data["E"].to_list(), name = title, index = thing_data["quarter"])
        # print(data_series)

        monte_files = pandas.concat([monte_files, m_data.to_frame().T])
        # eonte_files = pandas.concat([eonte_files, e_data.to_frame().T])


monte_files = monte_files.T
# monte_files = monte_files / monte_files.sum(axis=0)
# monte_files.index = list(thing_data.index)
# eonte_files = eonte_files.T


# Everything should be the same size that gets dumped into the folder, so I can take the last of the histbins, which
# is actually the dimensions of the lattice. Split that along x, since it's LxL, then square L (after changing it to an
# int), and that's how many bins I'll need.
histbins = re.split("x", histbins)[0]
histbins = int(histbins) * int(histbins)



# legends = []
sca = go.Figure()
his = go.Figure()
for title in list(monte_files.columns):
    his.add_trace(go.Histogram(x = monte_files[title], name = title, nbinsx=histbins))
    sca.add_trace(go.Scatter(x = list(monte_files.index), y = monte_files[title], name = title))
    # legends.append(title)

# print(monte_files[title])

his.update_layout(barmode="overlay", title = "Distribution of m - quarter sweeps")
his.update_traces(opacity=0.75)
his.update_yaxes(showticklabels=False, visible=False)
his.update_xaxes(title_text = "m")


sca.update_layout(title = "m per sweep")
sca.update_xaxes(title_text = "[Quarter] Sweep")
sca.update_yaxes(title_text = "m")

# his.show()
# plotly.io.write_image(his, "Hist_mag_20x20.pdf", format="pdf")

# print(monte_files)

his.show()
sca.show()


# # legends = []
# sca = go.Figure()
# for title in list(eonte_files.columns):
#     sca.add_trace(go.Scatter(x = list(eonte_files.index), y = eonte_files[title], name = title))

# sca.update_layout(title = "E per sweep")
# sca.update_xaxes(title_text = "[Quarter] Sweep")
# sca.update_yaxes(title_text = "E")

# sca.show()
