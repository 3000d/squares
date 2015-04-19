/**
 * Created by gberger on 15/04/15.
 */
var sketch = function(p){
  var socket;
  var SERVO_COUNT = 16;
  var slider_values = [];
  var sliders = [];
  var threshold = 10;
  p.setup = function(){

    socket = io().connect('http://localhost:3000');
    p.createCanvas(window.innerWidth, window.innerHeight);
    p.background(0);
    p.fill(255);
    p.stroke(255);

    for(var i=0;i<SERVO_COUNT;i++){
      var input = p.createSlider(0, 169, 1);
      input.attribute('id',i);
      input.position(10,(i+1)*30);
      sliders[i] = input;
      slider_values[i] =0;
    }
  };

  p.draw = function(){
    for(var i = 0; i<SERVO_COUNT;i++ ){
      var value = sliders[i].value();

      if(value != slider_values[i]){
        slider_values[i] = value;
        var id = sliders[i].attribute('id');
        var  data = {id:id, value:value};
        socket.emit('setmove', data);
      }
    }
  };
};

var gsSketch = new p5(sketch, 'p5-container');