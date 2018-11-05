var is_in_top_musics = false;

function get_html_cover_tag(music_info, percentage = false){
	 var html_tag = "<img class='cover' ";

	 if(percentage){
	 	html_tag += "data-percentage='" + percentage + "%'";
	 }

	 html_tag += "data-artist='" + music_info[0] + "' data-title='" + music_info[1] + "' src='" + music_info[3] + "' />";

	 return html_tag;
}

$(window).load(function(){

	$('#search').keyboard({ layout: 'qwerty' });

	$('.coverflow').coverflow({
		index: 			0,
		change:			function(event, cover) {
			var img = $(cover).children().andSelf().filter('img').last();
			$('#artist_name').text(img.data('artist') || 'unknown');
			$('#music_name').text(img.data('title') || 'unknown');
			$('#percentage_votes').html(img.data('percentage') || '&nbsp;');
		}
	});

	$('.coverflow').coverflow('option', 'enableClick', true);


	var vote_message = document.getElementById("vote_message");
	var not_found_message = document.getElementById("not_found_message");
	var none_voted_message = document.getElementById("none_voted_message");
	
	$('#close_vote_message').click( function() {
	    vote_message.style.display = "none";
	});
	
	$('#close_not_found_message').click( function() {
	    not_found_message.style.display = "none";
	});
	
	$('#close_none_voted_message').click( function() {
	    none_voted_message.style.display = "none";
	});

	var saved = decookienize("all_musics");
	if(saved==false){
		var all_musics = [["Mumford & Sons", "Believe", "Alternativo", "icons/z-m.jpg", 0], 
					  	 ["Snow Patrol", "Chasing Cars", "Alternativo", "icons/snow.jpg", 0],
					  	 ["Years and Years", "Take Shelter", "Eletronica", "icons/y.jpg", 0], 
					  	 ["Arcade fire", "Wake Up", "Alternativo", "icons/su.jpg", 0],
					  	 ["Aqua" ,"Barbie Girl", "Pop", "icons/z-a.jpg", 0]];
	} else{
		var all_musics = saved;
	}


	function get_most_voted(){
		var top_musics = all_musics.slice();
		top_musics.sort(function(music_info1, music_info2){return music_info2[4] - music_info1[4]});
		return top_musics[0][0] + ' - ' + top_musics[0][1];
	};


	$('#top_music_button').click(function(){
		$('.coverflow').empty();
		
		is_in_top_musics = true;
		var none_voted = true;
		var none_voted_message = document.getElementById("none_voted_message");
		var top_musics = all_musics.slice();
		var total_votes = top_musics.reduce(function(sum, music_info){return sum + music_info[4]}, 0);
		top_musics.sort(function(music_info1, music_info2){return music_info2[4] - music_info1[4]})

		for(var i = 0, len=top_musics.length; i< len; i++){
			if(top_musics[i][4] !== 0){
				none_voted = false;
				$('.coverflow').append(get_html_cover_tag(top_musics[i], Math.round(top_musics[i][4]*100/total_votes)));
			}
		}
		$('.coverflow').coverflow('index', 0);
		$('.coverflow').coverflow('refresh');
		if(none_voted){
			$('#all_button').trigger('click');
			none_voted_message.style.display = "block";	
		}
	});

	$('#all_button').click(function(){
		is_in_top_musics = false;

		$('.coverflow').empty();
		for(var i = 0, len=all_musics.length; i< len; i++){
			$('.coverflow').append(get_html_cover_tag(all_musics[i]));
		}
		$('.coverflow').coverflow('index', Math.floor(all_musics.length / 2));
		$('.coverflow').coverflow('refresh');
	});

	$('#submit').click(function(){
		is_in_top_musics = false;

		var search_term = document.getElementById("search").value;
		var message_not_found = document.getElementById("not_found_message");
		var found= false;
		
		$('.coverflow').empty();
		for(var i = 0, len=all_musics.length; i< len; i++){
			if(all_musics[i][0].toLowerCase().indexOf(search_term.toLowerCase()) !== -1 || 
			   all_musics[i][1].toLowerCase().indexOf(search_term.toLowerCase()) !== -1 ||
			   all_musics[i][2].toLowerCase().indexOf(search_term.toLowerCase()) !== -1){
				$('.coverflow').append(get_html_cover_tag(all_musics[i]));
				found = true;
			}
		}
		
		$('.coverflow').coverflow('index', 0);
		$('.coverflow').coverflow('refresh');
		if(!found){
			$('#all_button').trigger('click');
			message_not_found.style.display = "block";	
		}
	});


	$('#all_button').trigger('click');


	//when user clicks on Votar
	$('#vote_button').click( function(){
		vote_message.style.display = "block";
		var voted = $('.coverflow').coverflow('cover');
		all_musics.find(function(music_info){ 
							return music_info[0] === voted.data('artist') &&
								   music_info[1] === voted.data('title');
						})[4]++;
		if( is_in_top_musics ){
			$('#top_music_button').trigger('click');
		}
		cookienize(all_musics, "all_musics");
	});

	music_ballon_string = "<b>Current Music:</b> <br> Mumford & Sons - Believe <br> <b>Next Music:</b> <br>" + get_most_voted();
	console.log(music_ballon_string)
	// A tocar icon funcionality
	$('#musicPlaying_button').balloon({ contents: music_ballon_string,
										tipSize: 30,
                                    	showDuration: "slow",
                                    	showAnimation: function(d, c) { this.fadeIn(d, c);},
                                    	css: {
                                        	border: 'solid 4px #5baec0',
                                        	boxShadow: "2px 2px 2px #888",
                                       		padding: '10px',
                                        	fontFamily: 'Open Sans',
                                        	fontSize: '150%',
                                        	lineHeight: '2',
                                        	backgroundColor: '#eaf8fc',
                                        	color: '#2b4d6e'
                                    }
                                });


});

