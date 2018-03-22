

document.onkeydown = checkkey;
function checkkey(e){
	e = e || window.event;
	if(e.keyCode == '38'){
		draw("up");
	}else if(e.keyCode == '40'){
		draw("dn");
	}else if(e.keyCode == '37'){
		draw("rg")
	}else if(e.keyCode == '39'){
		draw("lf")
	}
}
var x = 400;
var y = 350;
//function fr drawing stuf
function draw(key){
	var canvas = document.getElementById("tyOutCanvas");
	var ctx = canvas.getContext("2d");
	ctx.moveTo(x,y);
	switch(key){
		case 'up':
			y--; 
			ctx.lineTo(x,y);
			ctx.stroke();
		break;
		case 'dn':
			y++;
			ctx.lineTo(x,y);
			ctx.stroke();
		break;
		case 'rg':
			x--;
			ctx.lineTo(x,y);
			ctx.stroke();
		break;
		case 'lf':
			x++;
			ctx.lineTo(x,y);
			ctx.stroke();
		break;
	}
}