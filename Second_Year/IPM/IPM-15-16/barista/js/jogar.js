var shown = false;

$(document).ready(function() {
 
 var tables_string = " <div class='balloon' id = 'invite_options_div'>" +
						"<div id='tables_title'> Escolha uma mesa </div>" +
						"<div id = 'tables_div'>" +
						"<button class='button' id='table1_button' onclick='invite()'>" +
						"<div class ='table_button_div'>" +
						"<img class='main_icons' src='icons/tables.png'>" +
						"</div>"+
						"<figcaption class='tables_text'> Mesa 1 </figcaption>" +
						"</button>" +
						"<button class='button tables' id='table2_button' onclick='invite()'>" +
						"<div class = 'table_button_div'>"+
						"<img class='main_icons' src='icons/tables.png'>" +
						"</div>"+
						"<figcaption class='tables_text'> Mesa 2 </figcaption>" +
						"</button>"+
						"<button class='button tables' id='table3_button' onclick='invite()'>" +
						"<div class = 'table_button_div'>"+
						"<img class='main_icons' src='icons/tables.png'>" +
						"</div>"+
						"<figcaption class='tables_text'> Mesa 3 </figcaption>" +
						"</button>" +
						"<button class='button tables' id='table4_button' onclick='invite()'>" +
						"<div class = 'table_button_div'>"+
						"<img class='main_icons' src='icons/tables.png'>" +
						"</div>"+
						"<figcaption class='tables_text'> Mesa 4 </figcaption>" +
						"</button>"+
						"<button class='button tables' id='table5_button' onclick='invite()'>" +
						"<div class = 'table_button_div'>"+
						"<img class='main_icons' src='icons/tables.png'>" +
						"</div>"+
						"<figcaption class='tables_text'> Mesa 5 </figcaption>" +
						"</button>"
						"</div> </div>";
						
	$(function() {
		$('#invite_button').on("click", function() {
		shown ? $(this).hideBalloon() : $(this).showBalloon({contents: tables_string,
															 showDuration: "slow",
															 position: 'left',
															 offsetX:-350,
															 offsetY: -320 ,
															 css:{
																opacity: 1,
															 }
															 });
		shown = !shown;
		});
	});
	
});

function invite(){
		 console.log("OLAAA");
		 /*$('#tables_tilte').css("-webkit-animation", "blurFadeInOut 3s ease-in backwards");
		$('#tables_tilte').css(	"-moz-animation", "blurFadeInOut 3s ease-in backwards");
		$('#tables_tilte').css("-ms-animation", "blurFadeInOut 3s ease-in backwards");
		$('#tables_tilte').css("-animation", "blurFadeInOut 3s ease-in backwards");*/
		$('#tables_title').text("A convidar...");
		setTimeout(accepted,5000);
	}
function accepted(){
	$('#invite_button').hideBalloon();
	shown = false;
	$('#invite_accepted_box').css('display','block');
}
$('#inicialize_button').click(function(){
	$('#invite_accepted_box').css('display','none');
});

$('#cancel_button').click(function(){
	$('#invite_accepted_box').css('display','none');
});


$('#terminar_button').click(function(){
	$('#end_game_box').css('display','block');
});

$('#drink_shortcut_button').click(function(){
	location.href='pedir.html';
});

$('#homepage_shortcut_button').click(function(){
	location.href='barista.html';
});