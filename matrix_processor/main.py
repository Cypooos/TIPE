from reader import Interpretor
from matrix import Matrix, to_matrix
from distance_test import get_dist_max,get_dist_max_mem


def test_arithmetics():
	arithmetics = Interpretor("algos/arithmetics.asm")
	arithmetics.run([
		Matrix.from_table([[1, 2], [3, 4]]),
		Matrix.from_table([[5, 6], [7, 8]])
	])

def test_is_cycle():

	get_cycle = Interpretor("algos/is_cycle.asm")

	tests_cycl = [
		to_matrix("0 1\n1 0"),
		to_matrix("0 1\n1 2\n2 3\n3 4\n4 5\n5 0"),
		Matrix.from_table([[0, 0, 0, 1], [1, 0, 0, 0], [1, 0, 1, 0], [0, 0, 0, 0]]),
		to_matrix("0 1\n1 2\n2 4\n1 4\n4 3\n2 5\n5 3\n3 6\n6 7\n7 5"),
	]

	test_acycl = [
		to_matrix("0 1"),
		to_matrix("0 2\n2 1\n1 3"),
		to_matrix("0 2\n2 1\n1 3\n0 4"),
		to_matrix("0 1\n1 2\n2 3\n0 2\n0 3\n1 3"),
		to_matrix("0 1\n1 2\n2 4\n1 4\n4 3\n2 5\n5 3\n3 6\n6 7"),
		to_matrix("0 1\n0 2\n0 3\n1 4\n1 5\n2 6\n2 7\n4 8"),
		to_matrix("0 2\n0 4\n0 5\n1 4\n1 5\n2 3\n2 4\n4 5"),
	]

	print("\n\n --- TEST CYCLIQUE ---\n")
	for i,x in enumerate(tests_cycl):
		if get_cycle.run([x]) == 1:
			print(" -- Test #"+str(i)+" correct.\n")
		else:
			print(" -- Test #"+str(i)+" INCORRECT.\n")
			exit()
		
	print("\n\n --- TEST ACYCLIQUE ---\n")
	for i,x in enumerate(test_acycl):
		if get_cycle.run([x]) == 0:
			print(" -- Test #"+str(i)+" correct.\n")
		else:
			print(" -- Test #"+str(i)+" INCORRECT.\n")
			exit()

def test_distance():
	distance = Interpretor("algos/distance.asm")

	tests = [
		to_matrix("0 1"),
		to_matrix("0 2\n2 1"),
		to_matrix("0 2\n2 3\n3 1"),
		to_matrix("\n".join([str(x+1)+" " +str(x) for x in range(1,100-1)]+["0 "+str(100-1)]))
	]


	for i,test in enumerate(tests):
		returned = distance.run([test])
		ret = get_dist_max(test)[0]
		if returned.data[0][0] != ret:
			print("Test #"+str(i)+" failed:\nget_dist_max(mat) = "+str(ret)+"\nwhile run(mat) = "+str(returned.data[0][0]))
		else:
			print("Test #"+str(i)+" sucessful")

def test_distance_mem():
	distance = Interpretor("algos/distance_mem.asm")

	tests = [
		to_matrix("0 1"),
		to_matrix("0 2\n2 1"),
		to_matrix("0 2\n2 3\n3 1"),
		to_matrix("\n".join([str(x+1)+" " +str(x) for x in range(1,100-1)]+["0 "+str(100-1)]))
	]


	for i,test in enumerate(tests):
		returned = distance.run([test])
		ret = get_dist_max_mem(test)[0]
		if returned.data[0][0] != ret:
			print("Test #"+str(i)+" failed:\nget_dist_max_mem(mat) = "+str(ret)+"\nwhile run(mat) = "+str(returned.data[0][0]))
		else:
			print("Test #"+str(i)+" sucessful, distance is", ret)


if __name__ == "__main__":
	test_distance_mem()