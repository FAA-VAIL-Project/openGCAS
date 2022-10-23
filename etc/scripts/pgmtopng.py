###
### Created by Jack D.V. Carson on 10/22/2022.
### Copyright (C) GNU LESSER GENERAL PUBLIC LICENSE
###

# Convert Pilot greymap image of rqsDataBlock to PNG
# Requres "pnmtopng"
# sudo apt-get install pnmtopng -y

import os

directory = "../output_vis";

index = 0;
for filename in os.listdir(directory):
    f = os.path.join(directory, filename);
    if ".pgm" in filename:
        s = f"{filename[0]}vis.png"
        f2 = os.path.join(directory, s);
        print(s);
        print(filename)
        os.system(f"pnmtopng {f} > {f2}");
        index += 1;