from genericpath import isfile
import plotly.express as px
import plotly.graph_objects as go
import plotly
import pathlib
import pandas
import re

# with open(pathlib.Path.cwd() / "magMag_Temp_5.000000_Size_20x20_seed_639699332.csv") as infile:
#     data = pandas.read_csv(infile, header=0)

# # # print(data)

# # fig = px.scatter(data, x = "sweep", y = "m", title = "")
# # plotly.io.write_image(fig, "magT2L100x100.pdf", format="pdf")
# # # fig.show()

# his = px.histogram(data, x = "m")
# # plotly.io.write_image(his, "Hist_magT2L100x100.pdf", format="pdf")
# his.show()

# # For the histogram: do it for a bunch of different temps. You should get a gausian at high temp, two seperate clumps
# # at low temp

# Combine Files
csv_folder = pathlib.Path.cwd() / "DataFiles"

# temps = []
monte_files = pandas.DataFrame()
for thing in csv_folder.iterdir():
    # print(type(thing))
    if thing.is_file() and thing.suffix in ".csv":
        # print(thing)
        thingy = re.split("_", str(thing))
        # temp = re.sub(r"\.", ",", thingy[2])
        temp = re.sub(r"000$", "", thingy[2])
        temp = f"{temp} K"
        # print(temp)

        with open(thing) as thing_file:
            thing_data = pandas.read_csv(thing_file, header = 0)

        m_data = thing_data["m"].to_list()

        data_series = pandas.Series(m_data, name = temp)
        # print(data_series)

        monte_files = pandas.concat([monte_files, data_series.to_frame().T])

monte_files = monte_files.T


# legends = []
his = go.Figure()
for title in list(monte_files.columns):
    his.add_trace(go.Histogram(x = monte_files[title], histnorm = 'probability density', name = title, nbinsx=500))
    # legends.append(title)


# for 
# his.add_trace(go.Histogram(monte_files))
his.update_layout(barmode="overlay", title = "<m> at different temps")
# his.update_l
his.update_traces(opacity=0.75)
his.update_yaxes(showticklabels=False, visible=False)
his.update_xaxes(title_text = "<m>")
his.show()
plotly.io.write_image(his, "Hist_mag_20x20.pdf", format="pdf")

# print(monte_files)

# his = px.histogram(monte_files, nbins=250)
# his.show()

        # third one is the 
# print(temp)
