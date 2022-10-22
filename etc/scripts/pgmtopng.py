# Convert Pilot greymap image of rqsDataBlock to PNG
# Requres "pnmtopng"
# sudo apt-get install pnmtopng -y

import os

directory = "../output_vis";

index = 0;
for filename in os.listdir(directory):
    f = os.path.join(directory, filename);
    s = f"{index}vis.png"
    f2 = os.path.join(directory, s);
    os.system(f"pnmtopng {f} > {f2}");
    index += 1;