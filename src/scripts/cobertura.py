import re
import sys


def get_useful_prefetches(memreport):
	result = {}
	name = ""
	for line in memreport:
		m = re.match("\[ ([id]?l[0-9])-0 \]",line)
		if m:
			name = m.group(1)
	
		m = re.match("Useful Prefetches = ([0-9]+)",line)
		if m:
			useful_pref = m.group(1)
			result[name] = result.get(name,0) + float(useful_pref)	
	return result


def get_misses_without_prefetch(memreport):
        result = {} 
	for line in memreport:
                m = re.match("\[ ([id]?l[0-9])-0 \]",line)
                if m:
                        name = m.group(1)
        
                m = re.match("Misses = ([0-9]+)",line)
                if m:
                        misses = m.group(1)
                        result[name] = result.get(name,0) + float(misses)
	return result


def calc_coverage(useful_pref, misses):
	result = []
	for name1,name2 in zip(useful_pref,misses):
		if name1 != name2:
			print('ERROR: Cache level mismatch')
			exit(-1)
		if misses[name2] == 0: 
			misses[name2] = useful_pref[name1]
		result.append((name1, useful_pref[name1] / misses[name2]))
	return result


def main():
	if len(sys.argv) != 3:
		print("USAGE: program memreport_with_prefetch memreport_without_prefetch")
		exit(1)
	memreport_pref = open(sys.argv[1])
	memreport_nopref = open(sys.argv[2])

	useful_pref = get_useful_prefetches(memreport_pref)
	misses = get_misses_without_prefetch(memreport_nopref)
	
	print(useful_pref)
	print(misses)

	coverage = calc_coverage(useful_pref, misses)
	print(coverage)

if __name__ == "__main__":
    main()
