window.socket = io().connect('http://localhost:3000');

$(function() {
  var $container = $('#calib-container');

  var SQUARE_COUNT = 16;

  function init() {

    for(var i = 0; i < SQUARE_COUNT; i++) {
      createSquareForm(i);
    }
  }

  var createSquareForm = function(id) {
    var $formContainer = $("<form />");
    var $span     = $("<span/>").html("square " + pad(id, 2) + ' : ');
    var $minValue = $("<input/>").attr('type', 'number').attr('id', 'min-'+id).attr('value', 200);
    var $maxValue = $("<input/>").attr('type', 'number').attr('id', 'max-'+id).attr('value', 450);
    var $submit   = $("<button/>").attr('id', 'submit-' + id).html('OK');

    $submit.on('click', function(e) {
      onSubmit(id, $minValue.val(), $maxValue.val());

      return false;
    });

    $formContainer.append($span).append($minValue).append($maxValue).append($submit);
    $container.append($formContainer);
  };

  var onSubmit = function(id, min, max) {
    socket.emit('setcalib', {
      square: id,
      min: min,
      max: max
    });
  };

  function pad(num, size) {
    var s = "0" + num;
    return s.substr(s.length-size);
  }

  socket.on('getcalib', function(data) {
    $('#min-' + data.square).attr('value', data.min);
    $('#max-' + data.square).attr('value', data.max);
  });

  init();
});