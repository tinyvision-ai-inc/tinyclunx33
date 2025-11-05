import sys

def read_tsv(path) -> dict:
    table = {}
    with open(path) as f:
        for line in f:
            row = line.strip().split('\t')
            table[int(row[0])] = { "src": row[1:] }
    return table

def parse_pinout_table(table) -> None:
    for k, v in table.items():
        v['pin_name'] = v['src'][0]

def parse_pod_table(table) -> None:
    for k, v in table.items():
        v['pin_name'] = v['src'][0]
        v['pin_signal'] = v['src'][1] if len(v['src']) > 1 else '-'

def merge_pod_table(pinout, pod, name) -> None:
    for k, v in pod.items():
        n1 = pinout[k]['pin_name'].lower()
        n2 = v['pin_name'].lower()
        if not n1.startswith(n2) and not n1.endswith(n2):
            print(f'warn: {n1} and {n2} do not match')
        pinout[k][name] = v['pin_signal']

def main(argv):
    pinout = read_tsv(argv[1])
    parse_pinout_table(pinout)
    for path in argv[2:]:
        pod = read_tsv(path)
        parse_pod_table(pod)
        merge_pod_table(pinout, pod, path)
    print('\t\t', end='')
    print('\t'.join(s[:-4] for s in argv[2:]))
    for k, v in pinout.items():
        print(k, end='\t')
        print(v['pin_name'], end='\t')
        print('\t'.join([v[f] if f in v else '-' for f in argv[2:]]))

if __name__ == "__main__":
    main(sys.argv)
