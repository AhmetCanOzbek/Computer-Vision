import os
import random
import shutil
import sys

#remove and make a new one
if os.path.isdir('sub_dataset'):
	shutil.rmtree('sub_dataset')

os.mkdir('sub_dataset')
os.chdir('sub_dataset')
os.mkdir('train')
os.mkdir('test')
os.chdir('train')
os.mkdir('empty')
os.mkdir('occupied')
os.chdir('..')
os.chdir('test')
os.mkdir('empty')
os.mkdir('occupied')

os.chdir('/Users/ahmetcanozbek/Desktop/Parking/PKLot/PKLotSegmented')
#n_files
n_train_empty = 800
n_train_occupied = 800

n_test_empty = 150
n_test_occupied = 150

n_empty = n_train_empty + n_test_empty
n_occupied = n_train_occupied + n_test_occupied


#Construct pool
#*empty
ignore = ['Occupied'] #Ignore Occupied
files_list_empty = []
counter = 0
#Index all empty images
for path, dirs, files in os.walk("."):
	dirs[:] = [dn for dn in dirs if dn not in ignore]
	for filename in files:
		if filename.endswith(".jpg"):			
			files_list_empty.append(os.path.join(path, filename))
			sys.stdout.write('\r')
			sys.stdout.write(str(counter))
			sys.stdout.flush()	
			counter += 1

print " "
print "Empty images indexing done."


#*occupied
ignore = ['Empty'] #Ignore Empty
files_list_occupied = []
counter = 0
#Index all occupied images
for path, dirs, files in os.walk("."):
	dirs[:] = [dn for dn in dirs if dn not in ignore]
	for filename in files:
		if filename.endswith(".jpg"):			
			files_list_occupied.append(os.path.join(path, filename))
			sys.stdout.write('\r')
			sys.stdout.write(str(counter))
			sys.stdout.flush()	
			counter += 1

print " "
print "Occupied images indexing done."



#Train - Test split
#Randomly sample
files_list_empty = random.sample(files_list_empty,n_empty)
files_list_occupied = random.sample(files_list_occupied,n_occupied)

random.shuffle(files_list_empty)
files_list_empty_train = files_list_empty[:n_train_empty]
files_list_empty_test = files_list_empty[n_train_empty:]

random.shuffle(files_list_occupied)
files_list_occupied_train = files_list_occupied[:n_train_occupied]
files_list_occupied_test = files_list_occupied[n_train_occupied:]

print "Random shuffling done."

#Start copying
#Train empty
for file in files_list_empty_train:
	shutil.copy2(file, '/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/sub_dataset/train/empty')
#Train occupied
for file in files_list_occupied_train:
	shutil.copy2(file, '/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/sub_dataset/train/occupied')
#Test empty
for file in files_list_empty_test:
	shutil.copy2(file, '/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/sub_dataset/test/empty')
#Test occupied
for file in files_list_occupied_test:
	shutil.copy2(file, '/Users/ahmetcanozbek/Desktop/Parking/MatlabImplementation/sub_dataset/test/occupied')

print "Copying image files done."

