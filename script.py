import sys,getopt
import os
import urllib2
import urllib
public_hash     = 'jq3War2WjvINM8dRljK5'
private_hash    = 'zze167D184HvXDwbKRjl'
base_url        = 'https://data.sparkfun.com'
post_url        = base_url + '/input/' + public_hash


headers = {
	'Content-type': 'application/x-www-form-urlencoded',
	'Phant-Private-Key': private_hash
}
data={}

def main(argv):
	try:
		opts, args = getopt.getopt(argv[1:],"hb:t:x:y:z:p:m:l:w:",["help","battery=","temperature=","x_acc=","y_acc=","z_acc=","pressure=","mac=","luminosity=","watermark="])
	except getopt.GetoptError:
		print "help: python %s -h or python %s --help"%(argv[0],argv[0])
		sys.exit(2)
	for opt, arg in opts:
		if opt in ("-h","--help"):
			print 'Syntaxe: python %s [args...]\n'
			print '-b arg | --battery arg : la valeur du battery\n'
			print '-t arg | --temperature arg : la valeur de la temperature\n'
			print '-x arg | --x_acc arg : la valeur de coordonnee x de l`accelerometre\n'
			print '-y arg | --y_acc arg : la valeur de coordonnee y de l`accelerometre\n'
			print '-z arg | --z_acc arg : la valeur de coordonnee z de l`accelerometre\n'
			print '-p arg | --pressure arg : la valeur de la pression\n'
			print '-m arg | --mac arg : l`adresse mac du mote\n'
			print '-l arg | --luminosity arg : la valeur de la luminosite\n'
			print '-w arg | --watermark arg : la valeur de la humidite\n'
			sys.exit()
		elif opt in ("-b", "--battery"):
			data["battery"] = int(arg)
		elif opt in ("-t", "--temperature"):
			data["temperature"] = int(arg)
		elif opt in ("-x", "--x_acc"):
			data["x_acc"] = int(arg)
		elif opt in ("-y", "--y_acc"):
			data["y_acc"] = int(arg)
		elif opt in ("-z", "--z_acc"):
			data["z_acc"] = int(arg)
		elif opt in ("-p", "--pressure"):
			data["pressure"] = int(arg)
		elif opt in ("-m", "--mac"):
			data["mac"] = str(arg)
		elif opt in ("-l", "--luminosity"):
			data["luminosity"] = int(arg)
		elif opt in ("-w", "--watermark"):
			data["watermark"] = int(arg)

if __name__ == "__main__":
	if len(sys.argv) < 2 :
		print "help: python %s -h or python %s --help"%(sys.argv[0],sys.argv[0])
		sys.exit(2)
	main(sys.argv)
	encoded_args = urllib.urlencode(data)
	#print encoded_args
	post_request = urllib2.Request(post_url,encoded_args,headers)
	post_response = urllib2.urlopen(post_request)
	print post_response.read()
	get_request = urllib2.Request("http://196.203.130.75/mongodb/index.php?%s"%(encoded_args))
	urllib2.urlopen(get_request)
