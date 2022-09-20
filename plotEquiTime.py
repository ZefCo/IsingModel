import pathlib
import pandas
import plotly.graph_objects as go
import re
import sys

cirtical_folder = pathlib.Path.cwd() / "DataFiles" / "2_3Eq"
# try:
#     time_max = int(sys.argv[1])
# except IndexError as e:
#     time_max = 1000

# try:
#     time_min = int(sys.argv[2])
# except IndexError as e:
#     time_min = 0

# if time_min > time_max:
#     time_dump = time_min
#     time_min = time_max
#     time_max = time_dump


# critical_frame = pandas.DataFrame()
# for file in cirtical_folder.iterdir():
#     # print(file)
#     with open(file) as local_crit:
#         local_frame = pandas.read_csv(local_crit, header=0)

#     critical_frame = pandas.concat([critical_frame, local_frame])

# timetemp = re.split(r"_", re.split(r"\\", str(file))[6])[2]
# timetemp = re.sub(r"00000$", "", timetemp)

# critical_frame = critical_frame.sort_index()
# critical_frame = critical_frame[(critical_frame["quarter"] < time_max) & (critical_frame["quarter"] > time_min)]

# sca = go.Figure(go.Scatter(x = critical_frame["quarter"], y = critical_frame["m"]))
# sca.update_layout(title = f"Time plot of {timetemp} K     tmin = {time_min}  tmax = {time_max}")
# sca.show()
# # print(critical_frame)



sca = go.Figure()
for file in cirtical_folder.iterdir():
    # print(file)
    with open(file) as local_file:
        local_frame = pandas.read_csv(local_file, header=0)
    # print(local_frame)

    # break
    sca.add_trace(go.Scatter(x = local_frame["quarter"], y = local_frame["m"]))

sca.show()

