let BOARD_SIZE;
let NUM_PLAYERS;
let NUM_SNEKS;
let NUM_LADDERS;
let NUM_ROLLS;
let PLAYERS = [];
const SNEK_POSITIONS = [];
const LADDER_POSITIONS = [];
const ROLLS = [];

function Player(index) {
  this.location = 'start'
  this.id = index + 1
}

function readInput() {
  BOARD_SIZE = nextInt();
  NUM_PLAYERS = nextInt();
  for (let i = 0; i < NUM_PLAYERS; ++i)
    PLAYERS[i] = new Player(i)
  NUM_SNEKS = nextInt();
  for (let i = 0; i < NUM_SNEKS; ++i)
    SNEK_POSITIONS[i] = {
      from: {
        x: nextInt() - 1,
        y: nextInt() - 1
      },
      to: {
        x: nextInt() - 1,
        y: nextInt() - 1
      }
    };
  NUM_LADDERS = nextInt();
  for (let i = 0; i < NUM_LADDERS; ++i)
    LADDER_POSITIONS[i] = {
      from: {
        x: nextInt() - 1,
        y: nextInt() - 1
      },
      to: {
        x: nextInt() - 1,
        y: nextInt() - 1
      }
    };
  NUM_ROLLS = nextInt();
  for (let i = 0; i < NUM_ROLLS; ++i) ROLLS[i] = nextInt() + nextInt();
}

function move(position, spaces) {
  if (position === 'start') {
    position = { x: 0, y: 0 };
    --spaces;
  }
  while (spaces > 0) {
    const odd = position.y % 2;
    for (let i = odd ? position.x-1 : position.x+1; odd ? i >= 0 : i < BOARD_SIZE; odd ? --i : ++i) {
      position.x = i;
      --spaces;
    }
    if (spaces > 0) {
      --spaces;
      ++position.y;
      if (position.y === BOARD_SIZE)
        return 'win';
    }
  }

  for (let i = 0; i < NUM_LADDERS; ++i)
    if (LADDER_POSITIONS[i].from.x === position.x && LADDER_POSITIONS[i].from.y === position.y)
      return { x: LADDER_POSITIONS[i].to.x, y: LADDER_POSITIONS[i].to.y }
  for (let i = 0; i < NUM_SNEKS; ++i)
    if (SNEK_POSITIONS[i].from.x === position.x && SNEK_POSITIONS[i].from.y === position.y)
      return { x: SNEK_POSITIONS[i].to.x, y: SNEK_POSITIONS[i].to.y }

  return position;
}

function main() {
  readInput()

  let roll = 0, p = 0;
  while (PLAYERS.length && roll < ROLLS.length) {
    PLAYERS[p].location = move(PLAYERS[p].location, ROLLS[roll++]);
    if (PLAYERS[p].location === 'win') {
      console.log(PLAYERS[p].id + ' winner');
      PLAYERS = [].concat(
        PLAYERS.slice(0, p),
        PLAYERS.slice(p+1)
      );
    }
    p = (p + 1) % PLAYERS.length;
  }

  for (let i = 0; i < PLAYERS.length; ++i)
    console.log(PLAYERS[i].id + ' ' + (PLAYERS[i].location.x+1) + ' ' + (PLAYERS[i].location.y+1));
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
