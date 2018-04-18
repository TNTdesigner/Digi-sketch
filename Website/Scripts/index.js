var release = 1960; //release datum van de originele Etch A Sketch

//omdat de tijd niet stil staat moet ook deze na gekeken worden
//bereken de leeftijd van de Etch A Sktech
function checkDate(){
	var nu = new Date();
	var leeftijd = (nu.getFullYear()) - release;
	document.getElementById("releaseDate").innerHTML = "A modern take on a " + leeftijd + " year old toy."
}