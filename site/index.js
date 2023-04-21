const updateFEN = function () {
  $('#fen').text('FEN: ' + game.fen());
};

const game = new Chess();

playerColor = 'w';

const onDrop = (source, target, piece) => {
  const move = game.move({ from: source, to: target, promotion: 'q' });
  if (move === null) {
    return 'snapback';
  }
  updateFEN();
  window.setTimeout(makeRandomMoveJS, 500);
};

function onDragStart(source, piece, position, orientation) {
  if (!piece.startsWith(playerColor)) {
    return false;
  }
}

function onSnapEnd() {
  board.position(game.fen());
  updateFEN();
}

function makeRandomMoveJS() {
  var possibleMoves = game.moves();
  if (game.game_over()) return;
  var randomIdx = Math.floor(Math.random() * possibleMoves.length);
  game.move(possibleMoves[randomIdx]);
  board.position(game.fen());
}

// Initialize the chessboard
const board = Chessboard('board', {
  draggable: true,
  dropOffBoard: 'snapback',
  orientation: 'white',
  position: 'start',
  onDrop: onDrop,
  onDragStart: onDragStart,
  onSnapEnd: onSnapEnd,
});
updateFEN();

// Start Position
$('#startBtn').on('click', function () {
  board.start(); game.reset();
  updateFEN();
});
