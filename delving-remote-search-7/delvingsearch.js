/**
 * @file delvingsearch.js
 *
 * TODO: Write description
 */

(function($) {	
	$(document).ready(function() {
		var width = $('#edit-query').width();
		var height = $('#edit-query').outerHeight();
		var offset = $('#edit-query').offset();
		var append = '<div id="clearsearch"><a href="javascript:void(0)" title="' + Drupal.t('Clear') + '">X</a></div>';
		$('#edit-query').before(append);
		var css = $('#edit-query').css('font-size');
		$('#clearsearch').css("font-size", css);
		css = {
			"width": width + 'px',
			"line-height": height - 8 + 'px',
		}
		$('#clearsearch').css(css);
		offset.left = offset.left+2;
		offset.top = offset.top+3;
		$('#clearsearch').offset(offset);
		$('#clearsearch a').live("click", (function() {
			$('#edit-query').val('');
			$('#clearsearch').hide();
			$('#edit-query').focus();
		}));
		$('#clearsearch').click(function() {
			$('#clearsearch').hide();
			$('#edit-query').focus();
		});
		$('#edit-query').focus(function() {
			$('#clearsearch').hide();
		});
		$('#edit-query').focusout(function() {
			$('#clearsearch').show();
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