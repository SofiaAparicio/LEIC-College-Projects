var ballon_shit = '<div style="position: absolute; height: 0px; width: 0px; border-width: 30px 17px 0px; border-style: solid; border-color: rgb(91, 174, 192) transparent transparent; -moz-border-top-colors: none; -moz-border-right-colors: none; -moz-border-bottom-colors: none; -moz-border-left-colors: none; border-image: none; top: 97px; left: 115px;"></div>' +
                   '<div style="position: absolute; height: 0px; width: 0px; border-width: 22px 13px 0px; border-style: solid; border-color: rgb(234, 248, 252) transparent transparent; -moz-border-top-colors: none; -moz-border-right-colors: none; -moz-border-bottom-colors: none; -moz-border-left-colors: none; border-image: none; top: 97px; left: 119px;"></div>'

function add_to_order(items) {
	var current = decookienize('order');
	if(current === false)
		cookienize(items, 'order');
	else{
		new_items = [];
		for(var i = 0; i < items.length; i++){
			var index = current.findIndex(function (element, index, array){ return element[0] === items[i][0] });
			if(index > -1)
				current[index][2] = parseInt(current[index][2]) + parseInt(items[i][2]);
			else
				new_items.push(items[i]);
		}
		cookienize(current.concat(new_items), 'order');
	}
}

function get_order(){
	var current = decookienize('order');
	return current === false ? [] : current;
}


$(function() {
   var shown = false;
   $('#bill_button').on("click", function() {
   		var order = get_order();

   		var order_string = "<div id='order_ballon_div'>"
   		for(var i = 0; i < order.length; i++)
   			order_string += "<span class='left'>" + order[i][2] + "x " + order[i][0] + "</span>" + 
   							"<span class='right'> &nbsp&nbsp€" + parseInt(order[i][2])*parseInt(order[i][1]) + 
   							"<span class='shortcut_add' data-index=" + i + "> <b> &nbsp; &#x2795 &nbsp; </b> </span></span>" + 
   							"<br>";
   		order_string += "</div>"

   		console.log(order_string);

    	shown ? $(this).hideBalloon() : $(this).showBalloon( {  contents: order_string,
    															tipSize: 30,
																showDuration: "slow",
																showAnimation: function(d, c) { this.fadeIn(d, c);},
																css: {
																	border: 'solid 4px #5baec0',
																	boxShadow: "2px 2px 2px #888",
																	padding: '10px',
																	fontFamily: 'Open Sans',
																	fontSize: '120%',
																	lineHeight: '2',
																	backgroundColor: '#eaf8fc',
																	color: '#2b4d6e'
																}
		});
		shown = !shown;
	});

	$(document).on("click", "span .shortcut_add", function() {

			var order = get_order();
			order[$(this).data('index')][2]++;
			cookienize(order, 'order');

			var order_string = "";
	   		for(var i = 0; i < order.length; i++)
	   			order_string += "<span class='left'>" + order[i][2] + "x " + order[i][0] + "</span>" + 
	   							"<span class='right'> &nbsp&nbsp€" + parseInt(order[i][2])*parseInt(order[i][1]) + 
	   							"<span class='shortcut_add' data-index=" + i + "> <b> &nbsp; &#x2795 &nbsp; </b> </span></span>" + 
	   							"<br>";

	   		$('#order_ballon_div').html(order_string);

	});
}); 
