

found = False

def d_search(tree, char, gChar, index):
	if (found):
		tree.left = Node(gChar)
		leftBT=BT[index]
		BT.push(leftBT)
		tree.left = NULL
		tree.right=Node(gChar)
		rightBT=BT[index]
		BT.push(rightBT)
	elif (tree.left):
		d_search(tree.left, char, gChar, index)
		if (tree.right):
			d_search(tree.right, char, gChar, index)

count = 0
def compare_infix(tree, infix):
	thisOne = True
	if (tree.left):
		thisOne = compare_infix(tree.left, infix)

	thisOne = thisOne and (tree.val == infix[count])
	count += 1
	tree.order = count
	if (tree.right):
		thisOne = compare_infix(tree.right, infix)
	return thisOne




prefix = raw_input("Stuff")
BT = [prefix[0]]


for char in prefix:
	for tree in BT:
		d_search(tree, char, prefix[1], 0)

for tree in BT:
	if (tree.size < infix.size): continue
	if (compare_infix(tree, infix)):
		print(tree)
		break

print("Done")