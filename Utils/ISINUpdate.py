import json
import csv
from pprint import pprint
import os.path

sourceFile='source.json'
targetFile='target.json'

ab = {}
with open('instrument.csv') as csv_file:
	csv_reader = csv.reader(csv_file,delimiter=',')
	for row in csv_reader:
		ab[row[0]] = row[1]
	print ab['BHP.AUS']
	
with open('source.txt') as sourceStream:
	data = json.load(sourceStream)
	for p in data["Server"]:
		configSymbols=p["ConfigSymbols"]
		for symbol in configSymbols:
			x = symbol["Symbol"]
			print(ab.get(x,'Not Found'))
			symbol["ISIN"] = ab.get(x,'Not Found')
	with open('target.json', 'w') as outfile:
		json.dump(data,outfile, indent=0)