

skyserver_query_file_name = "data/sequential_1000_queries.data"

data = [[float(x) for x in line.strip("\n").split()[2:]] for line in open(skyserver_query_file_name)]

# x1, y1, x2, y2

print(data[:5])

for i in range(len(data)):
    if len(data[i]) != 4:
        print(i)

# dim = 0,1
def sort_on_dim(data, dim, reverse=False):
    return sorted(data, key=lambda x: x[dim], reverse=reverse)


def sort_on_size_1_dim(data, dim, reverse=False):
    return sorted(data, key=lambda x: x[dim+2] - x[dim], reverse=reverse)


def sort_on_total_size(data, reverse=False):
    return sorted(data, key=lambda x: (x[2] - x[0]) * (x[3] - x[1]), reverse=reverse)


if __name__ == "__main__":
    files = []
    files.append(("data/skyserver_queries_reversed.data",
                  list(reversed(data))))
    files.append(("data/skyserver_sorted_dim_0.data",
                  sort_on_dim(data, 0)))
    files.append(("data/skyserver_sorted_dim_0_reversed.data",
                  sort_on_dim(data, 0, True)))
    files.append(("data/skyserver_sorted_dim_1.data",
                  sort_on_dim(data, 1)))
    files.append(("data/skyserver_sorted_dim_1_reversed.data",
                  sort_on_dim(data, 1, True)))
    files.append(("data/skyserver_sorted_size_dim_0.data",
                  sort_on_size_1_dim(data, 0)))
    files.append(("data/skyserver_sorted_size_dim_0_reversed.data",
                  sort_on_size_1_dim(data, 0, True)))
    files.append(("data/skyserver_sorted_size_dim_1.data",
                  sort_on_size_1_dim(data, 1)))
    files.append(("data/skyserver_sorted_size_dim_1_reversed.data",
                  sort_on_size_1_dim(data, 1, True)))
    files.append(("data/skyserver_sorted_total_size.data",
                  sort_on_total_size(data)))
    files.append(("data/skyserver_sorted_total_size_reversed.data",
                  sort_on_total_size(data, True)))
    for name, dat in files:
        print(name)
        for d in dat[:5]:
            print(d)
        with open(name, "w") as f:
            for x1, y1, x2, y2 in dat:
                f.write("2 9999999 {} {} {} {}\n".format(x1, y1, x2, y2))
