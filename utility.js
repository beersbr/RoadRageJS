Utility = {

	// This assumes that the arguments is a single object (array, dictionary)
	InheretFrom: function(context, obj, arguments){
		context.__proto__ = new obj(arguments);
		context.prototype = context.__proto__;

		return true;
	}
}