requestAnimFrame = (function() {
  return window.requestAnimationFrame ||
     window.webkitRequestAnimationFrame ||
     window.mozRequestAnimationFrame ||
     window.oRequestAnimationFrame ||
     window.msRequestAnimationFrame ||
     function(/* function FrameRequestCallback */ callback, /* DOMElement Element */ element) {
       window.setTimeout(callback, 1000/60);
     };
})();

Utility = {

	// This assumes that the arguments is a single object (array, dictionary)
	InheretFrom: function(context, obj, arguments){
		context.__proto__ = new obj(arguments);
		context.prototype = context.__proto__;

		return true;
	}
}