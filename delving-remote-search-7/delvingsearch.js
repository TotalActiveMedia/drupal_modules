/**
 * @file delvingsearch.js
 *
 * TODO: Write description
 */

(function($) {	
	$(document).ready(function() {
		var querylength = $('#edit-query').val();
		var height = $('#edit-query').outerHeight();
		var offset = $('#edit-query').offset();
		var append = '<div id="clearsearch"><a href="javascript:void(0)" title="' + Drupal.t('Clear') + '">' + querylength + '</a></div>';
		$('#edit-query').before(append);
		var css = $('#edit-query').css('font-size');
		$('#clearsearch').css("font-size", css);
		var width = $('#clearsearch').width();
		css = {
			"width": width + 15 + 'px',
			"line-height": height-8 + 'px',
		}
		$('#clearsearch').css(css);
		$('#clearsearch a').text('X');
		offset.left = offset.left+2;
		offset.top = offset.top+3;
		$('#clearsearch').offset(offset);
		$('#clearsearch a').live("click", (function() {
			$('#edit-query').val('');
			clearsearch = $('#clearsearch').remove();
			$('#edit-query').focus();
		}));
		$('#edit-query').focus(function() {
			clearsearch = $('#clearsearch').remove();
		});
		$('#edit-query').focusout(function() {
			$('#edit-query').before(clearsearch);
		});
		var modulepath = Drupal.settings.delvingsearch.modulepath;
		$('.delvingplacesorting').before('<img class="delvingsortabc" src="' + Drupal.settings.basePath + modulepath +
		'/img/abc.gif"><img class="delvingsort123" src="' + Drupal.settings.basePath + modulepath + '/img/123.gif">');
		$('.delvingsortabc').click(function() {
			var list = $(this).parent().find('li');
			$(list).tsort();
		});
		$('.delvingsort123').click(function() {
			var list = $(this).parent().find('li');
			$(list).tsort('.delvingcount', {order:'desc'});
		});
	})
})(jQuery);