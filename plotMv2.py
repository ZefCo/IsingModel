from genericpath import isfile
import plotly.express as px
import plotly.graph_objects as go
import plotly
import pathlib
import pandas
import re

csv_folder = pathlib.Path.cwd() / "DataFiles"

# histogram_frame = pandas.DataFrame()

for subfolder in csv_folder.iterdir():
    folder_frame = pandas.DataFrame()
    temp_name = re.sub(r"_", ".", str(subfolder.stem))

    # print(type(subfolder.iterdir()))
    # print(list(subfolder.rglob("*.csv")))
    if len(list(subfolder.rglob("*.csv"))) > 0:

        for subfile in subfolder.rglob("*.csv"):
            with open(subfile) as local_file:
                local_frame = pandas.read_csv(local_file, header=0, index_col=0)

            folder_frame = pandas.concat([folder_frame, local_frame])

        histbins = re.split(r"_", str(subfile))[5]
        histbins = re.split("x", histbins)[0]
        histbins = int(histbins)*int(histbins)

        # folder_frame = folder_frame.sort_index()
        # print((type(folder_frame["quarter", 0])))
        # folder_frame = folder_frame[folder_frame["quarter" > 1000000]]
        folder_frame = folder_frame.iloc[1000000:]
        # print(folder_frame)
        his = go.Figure(go.Histogram(x = folder_frame["m"], nbinsx = histbins))
        his.update_layout(title = f"M per spin @ {temp_name}")

        his.show()
        # plotly.io.write_image(his, pathlib.Path.cwd() / "DataFiles" / f"Hist_{temp_name}.pdf")




#     if thing.is_file() and thing.suffix in ".csv":
#         # print(thing)
#         thingy = re.split("_", str(thing))
#         # temp = re.sub(r"\.", ",", thingy[2])
#         temp = re.sub(r"000$", "", thingy[2])
#         temp = f"{temp}K"
#         # print(temp)
#         histbins = thingy[4]

#         title = f"{temp}_{str(thingy[6])}"

#         with open(thing) as thing_file:
#             thing_data = pandas.read_csv(thing_file, header = 0)

#         m_data = pandas.Series(thing_data["m"].to_list(), name = title, index = thing_data["quarter"])
#         e_data = pandas.Series(thing_data["E"].to_list(), name = title, index = thing_data["quarter"])
#         # print(data_series)

#         monte_files = pandas.concat([monte_files, m_data.to_frame().T])
#         eonte_files = pandas.concat([eonte_files, e_data.to_frame().T])


# monte_files = monte_files.T
# # monte_files = monte_files / monte_files.sum(axis=0)
# # monte_files.index = list(thing_data.index)
# eonte_files = eonte_files.T


# # Everything should be the same size that gets dumped into the folder, so I can take the last of the histbins, which
# # is actually the dimensions of the lattice. Split that along x, since it's LxL, then square L (after changing it to an
# # int), and that's how many bins I'll need.
# histbins = re.split("x", histbins)[0]
# histbins = int(histbins) * int(histbins)



# # legends = []
# sca = go.Figure()
# his = go.Figure()
# for title in list(monte_files.columns):
#     his.add_trace(go.Histogram(x = monte_files[title], name = title, nbinsx=histbins))
#     sca.add_trace(go.Scatter(x = list(monte_files.index), y = monte_files[title], name = title))
#     # legends.append(title)

# # print(monte_files[title])

# his.update_layout(barmode="overlay", title = "Distribution of m - quarter sweeps")
# his.update_traces(opacity=0.75)
# his.update_yaxes(showticklabels=False, visible=False)
# his.update_xaxes(title_text = "m")


# sca.update_layout(title = "m per sweep")
# sca.update_xaxes(title_text = "[Quarter] Sweep")
# sca.update_yaxes(title_text = "m")

# # his.show()
# # plotly.io.write_image(his, "Hist_mag_20x20.pdf", format="pdf")

# # print(monte_files)

# his.show()
# sca.show()


# # legends = []
# sca = go.Figure()
# for title in list(eonte_files.columns):
#     sca.add_trace(go.Scatter(x = list(eonte_files.index), y = eonte_files[title], name = title))

# sca.update_layout(title = "E per sweep")
# sca.update_xaxes(title_text = "[Quarter] Sweep")
# sca.update_yaxes(title_text = "E")

# sca.show()
