import re
import csv
from collections import defaultdict

csvfile = open("skyserver_01.csv")
output_file = open("data/skyserver_01.data", "w")
reader = csv.DictReader(csvfile)

successes = 0
failures = 0

counting = defaultdict(int)


for line in reader:
    res_dec = line["dec"]
    res_ra = line["#ra"]
    if res_ra is not None and res_dec is not None:
        successes += 1
        output_file.write("1 9999999 {} {}\n".format(res_ra, res_dec))
    else:
        print("FAILURE:", line["statement"])
        failures += 1

print(successes, failures)