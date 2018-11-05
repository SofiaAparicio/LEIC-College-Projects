
$(document).ready(function() {

	var por_pagar = get_order();

	var a_pagar_alcoolicas ={
		'Arizona Sunset' : ['Arizona Sunset',1, 3, 8],
		'Limonada' : ['Limonada',1, 2, 9]
	};


	var $stillToPay = $('#toPay-items');

	for(var item = 0; item< por_pagar.length; item++){
		var itemName = por_pagar[item][0],
		itemQuantity = por_pagar[item][2],
		itemPrice = por_pagar[item][1],
		itemImg = por_pagar[item][3],
		$template = $($('#PayTemplate').html());
		
		$template.find('.pay-product').css('background-image',itemImg) ;
		$template.find('h3').text(itemName);
		$template.find('.subtotal').text('$' + itemQuantity*itemPrice);
		$template.find('.quantity').text(itemQuantity);

		$template.data('price', itemPrice);
		$template.data('name', itemName);
		$template.data('subtotal', itemQuantity*itemPrice);
		$template.data('quantity', itemQuantity);
		$template.data('image',itemImg);
		$stillToPay.append($template);

	}
	$('body').on('click', '.pay-description', function() {
		var item = $(this)
		if(item.css('background-color') == 'rgb(31, 31, 46)')
			item.css('background-color',"");
		else
			item.css('background-color','rgb(31, 31, 46)');
	});

});

var total = 0;

function move_selected(right){
	if(right){
		var items1 = $('#toPay-items').children().toArray(),
			items2 = $('#PayContent').children().toArray()
			temp1 = '#PayContent';
			temp2 = '#toPay-items';
	}
	else{
		var items1 = $('#PayContent').children().toArray(),
			items2 = $('#toPay-items').children().toArray();
			temp1 = '#toPay-items';
			temp2 = '#PayContent';
	}
	for(var i=0;i<items1.length;i++){
		$template = $($('#PayTemplate').html());
		var not_matched = true;
		if($($(items1[i]).children('.pay-description')).css('background-color') == 'rgb(31, 31, 46)'){
			for(var j=0;j<items2.length;j++) {
				var item2 = $(items2[j]);
				if(item2.data('name') == $(items1[i]).data('name')){
					quantity = item2.data('quantity')+ 1;
					item2.find('.quantity').text(quantity);
					item2.data('quantity',quantity);
					upSubtotal(item2);
					not_matched = false;
				}
			}
			if(right)
				total= total + $(items1[i]).data('price');
			else
				total= total - $(items1[i]).data('price');
				
			$('#valueTotal').text(total);
			if(not_matched){
				$template.find('.pay-product').css('background-image',$(items1[i]).data('image')) ;
				$template.find('h3').text($(items1[i]).data('name'));
				$template.find('.subtotal').text('$' + $(items1[i]).data('price'));
				$template.find('.quantity').text(1);
				
				$template.data('price', $(items1[i]).data('price'));
				$template.data('subtotal', $(items1[i]).data('price'));
				$template.data('name', $(items1[i]).data('name'));
				$template.data('quantity',1);
				$template.data('image',$(items1[i]).data('image'));
				
				$(temp1).append($template);
			}
		$($(items1[i]).children('.pay-description')).css('background-color', '');
		quantity = $(items1[i]).data('quantity') - 1;
		$(items1[i]).data('quantity',quantity);
		$(items1[i]).find('.quantity').text(quantity);
		downSubtotal($(items1[i]));
			if(!quantity){
				$(items1[i]).remove();
			}
		}
	}
};

function upSubtotal(item) {
    var quantity = item.data('quantity'),
        price = item.data('price'),
        subtotal = quantity * price;
	item.find('.subtotal').text('$' + subtotal);
	item.data('subtotal', subtotal);
  }	
  
function downSubtotal(item) {
    var sub = item.data('subtotal'),
        price = item.data('price'),
        subtotal = sub - price;
	item.find('.subtotal').text('$' + subtotal);
	item.data('subtotal', subtotal);
  }	
  
 function pay(){
	$('.paymentSection').css('display', 'none');
	$('.title').text("Método de Pagamento");
	var raw_items = $('#toPay-items').children().toArray();
	var items = [];

	for(var i = 1; i < raw_items.length; i++)
		items.push([$(raw_items[i]).find('h3').text(),
					$(raw_items[i]).data('price'), 
					$(raw_items[i]).find('.quantity').text(),
					$(raw_items[i]).find('.cart-product').css('background-image')]);
	cookienize(items, 'order');
	$('#payment_div').css('display', 'block');
	
 }
 function pay_money(){
	$('#pay_money_message').css('display','block');
 }
 $('.close_pay').click(function(){
	$('#home_button').click();
});

function pay_atm(){
	$('#pay_atm_message').css('display','block');
 }
 $('.close_pay').click(function(){
	$('#home_button').click();
});