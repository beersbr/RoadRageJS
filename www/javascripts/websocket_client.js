GWebSocket = {
	socket: null,

	connect: function(){
		GWebSocket.socket = new WebSocket('ws://10.11.3.37:9898');

		GWebSocket.socket.onopen = function(){
			GWebSocket.socket.send(Game.player.to_json());
		}

		GWebSocket.socket.onmessage = function(msg){
			// console.log(msg.data);

			var cars = JSON.parse(msg.data);
			Game.cars = cars;
		};

		GWebSocket.socket.onclose = function(){
			GWebSocket.socket.send("close: "+Game.player.hash);
		}
	}
}