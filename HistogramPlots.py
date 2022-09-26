from genericpath import isfile
from threading import local
import plotly.express as px
import plotly.graph_objects as go
import plotly
import pathlib
import pandas
import re

hist_folder = pathlib.Path.cwd() / "Histogram"

# histogram_frame = pandas.DataFrame()

master_frame = pandas.DataFrame()
master_hist = go.Figure()
g = 1

for histfile in hist_folder.rglob("*.csv"):
    local_frame = pandas.read_csv(histfile, header=0)
    histbins, _ = local_frame.shape
    temp_name = re.sub(r"_", ".", str(histfile.stem))


    # master_frame = pandas.concat([master_frame, local_frame], axis = 1)

    master_hist.add_trace(go.Bar(x = local_frame["Bin"], y = local_frame["Count"], width = 0.005, name = temp_name))

    # local_frame = local_frame.to_dict()
    # print(histfile.stem)

    his = go.Figure(go.Bar(x = local_frame["Bin"], y = local_frame["Count"]))
    his.update_layout(title = f"<m> @ {temp_name}")

    g += 1

    his.show()
    # plotly.io.write_image(his, pathlib.Path.cwd() / "DataFiles" / f"Hist_{temp_name}.pdf")

master_hist.show()
# master_hist = go.Figure(go.Bar(x = ))
# print(master_frame)