import re
import csv
from collections import defaultdict

csvfile = open("skyserver_queries.csv")
output_file = open("data/skyserver_workload_10000_max_5_dup.data", "w")
reader = csv.DictReader(csvfile)

#  (p.ra BETWEEN 143.31411333333335 AND 143.31744666666668)

# example = "SELECT p.ra , p.dec, p.u, p.g, p.r, p.i, p.z, p.err_u, p.err_g, p.err_r, p.err_i, p.err_z, p.nMgyPerCount_u, p.nMgyPerCount_g, p.nMgyPerCount_r, p.nMgyPerCount_i, p.nMgyPerCount_z FROM PhotoObjAll p WHERE (p.ra BETWEEN 12.255606390744841 AND 12.27210140925516) AND (p.dec BETWEEN 0.19690739074484032 AND 0.2134024092551597) AND p.run = 4263 AND p.rerun = 301 AND p.camcol = 4 AND p.field = 197"


pattern_ra = re.compile(r"(p.)?ra (BETWEEN|between) [+\-]?(?:0|[1-9]\d*)(?:\.\d*)?(?:[eE][+\-]?\d+)? (AND|and) [+\-]?(?:0|[1-9]\d*)(?:\.\d*)?(?:[eE][+\-]?\d+)?")
pattern_dec = re.compile(r"(p.)?dec (BETWEEN|between) [+\-]?(?:0|[1-9]\d*)(?:\.\d*)?(?:[eE][+\-]?\d+)? (AND|and) [+\-]?(?:0|[1-9]\d*)(?:\.\d*)?(?:[eE][+\-]?\d+)?")
pattern_num = re.compile(r"[+\-]?(?:0|[1-9]\d*)(?:\.\d*)?(?:[eE][+\-]?\d+)?")  # re.compile(r"-?\d+(?:\.\d+)?")

print(pattern_num.findall("(p.dec BETWEEN -2.5009255159681026e-05 AND 0.01647000925515968)"))
print(pattern_num.findall("(p.dec BETWEEN -2.5009255159681026 AND 0.01647000925515968)"))

successes = 0
failures = 0

counting = defaultdict(int)

saved = set()
duplicates = 0

for line in reader:
    res_ra = pattern_ra.search(line["statement"])
    res_dec = pattern_dec.search(line["statement"])
    if res_ra is not None and res_dec is not None:
        ra_low, ra_high = pattern_num.findall(res_ra.group(0))
        dec_low, dec_high = pattern_num.findall(res_dec.group(0))
        tup = (ra_low, dec_low, ra_high, dec_high)
        if tup in saved:
            duplicates += 1
        if counting[tup] >= 5:
            continue
        counting[tup] += 1
        successes += 1
        output_file.write("2 9999999 {} {} {} {}\n".format(ra_low, dec_low, ra_high, dec_high))
        saved.add((ra_low, dec_low, ra_high, dec_high))
        if successes >= 10000:
            break
    else:
        print("FAILURE:", line["statement"])
        failures += 1

print(successes, failures, duplicates, len(saved))
