function Node(val) {
  this.val = val;
  this.left = this.right = null;
  this.order = this.depth = -1;
  this.size = 1;

  this.copy = function() {
    return Object.assign({}, this, {
      left: this.left && this.left.copy(),
      right: this.right && this.right.copy()
    });
  }
}

function main() {

  let infix, prefix, binaryTrees

  let found
  function depthFirstGenerate(node, char, gChar, index) {
    if (node.val === char) found = true
    function generate(direction) {
      node[direction] = new Node(gChar)
      const newNode = binaryTrees[index].copy()
      node[direction] = null
      newNode.size = binaryTrees[index].size + 1
      binaryTrees.push(newNode)
    }

    if (found) {
      generate('left')
      generate('right')
    }
    else {
      if (node.left) depthFirstGenerate(node.left, char, gChar, index)
      if (node.right) depthFirstGenerate(node.right, char, gChar, index)
      else generate('right')
    }
  }

  let count
  function compareInfix(node) {
    let thisOne = true
    if (node.left) thisOne = thisOne && compareInfix(node.left)
    thisOne = thisOne && (node.val === infix[count])
    node.order = ++count
    if (node.right) thisOne = thisOne && compareInfix(node.right)
    return thisOne
  }

  let correct
  while ((infix = nextString()) && (prefix = nextString())) {

    binaryTrees = [new Node(prefix[0])]

    let j = 0
    for (let i = 0; i < prefix.length - 1; ++i) {
      const char = prefix[i]
      const gChar = prefix[i+1]

      while (j < binaryTrees.length && (binaryTrees[j].size === i+1)) {
        found = false
        depthFirstGenerate(binaryTrees[j], char, gChar, j)
        ++j
      }
    }

    for (let i = 0; i < binaryTrees.length; ++i) {
      if (binaryTrees[i].size !== infix.length) continue
      count = 0
      if (compareInfix(binaryTrees[i])) {
        correct = binaryTrees[i]
        break
      }
    }

    const map = {}
    function assignDepth(node, depth) {
      node.depth = depth
      map[node.val] = node
      if (node.left) assignDepth(node.left, depth + 1)
      if (node.right) assignDepth(node.right, depth + 1)
    }
    assignDepth(correct, 0)
    let output = []
    for (let i = 0; i < infix.length; ++i) {
      const node = map[infix[i]]
      if (!output[node.depth]) output[node.depth] = []
      output[node.depth][i] = node.val
    }

    for (let i = 0; i < output.length; ++i) {
      for (let j = 0; j < output[i].length; ++j)
        process.stdout.write(output[i][j] ? output[i][j] + ' ' : '  ')
      console.log()
    }
  }
}

// default parsers for JS.
function nextInt() {
    return parseInt(nextString());
}

function nextFloat() {
    return parseFloat(nextString());
}

function nextString() {
    var next_string = "";
    clearWhitespaces();
    while (input_cursor < input_stdin.length && !isWhitespace(input_stdin[input_cursor])) {
        next_string += input_stdin[input_cursor];
        input_cursor += 1;
    }
    return next_string;
}

function nextChar() {
    clearWhitespaces();
    if (input_cursor < input_stdin.length) {
        return input_stdin[input_cursor++];
    } else {
        return '\0';
    }
}

process.stdin.resume();
process.stdin.setEncoding('ascii');

var input_stdin = "";
var input_cursor = 0;
process.stdin.on('data', function (data) { input_stdin += data; });
process.stdin.on('end', function () { main(); });

function isWhitespace(character) {
    return ' \t\n\r\v'.indexOf(character) > -1;
}

function clearWhitespaces() {
    while (input_cursor < input_stdin.length && isWhitespace(input_stdin[input_cursor])) {
        // ignore the next whitespace character
        input_cursor += 1;
    }  
}
