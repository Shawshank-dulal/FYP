!function(){var e,n,o,t,a,r,i,c,d,u,l,p=(o=[],t=document,a=t.documentElement,r=a.doScroll,i="DOMContentLoaded",c="addEventListener",d="onreadystatechange",l=(r?/^loaded|^c/:/^loaded|c/).test(t[u="readyState"]),t[c]&&t[c](i,n=function(){t.removeEventListener(i,n,!1),f()},!1),r&&t.attachEvent(d,n=function(){/^c/.test(t[u])&&(t.detachEvent(d,n),f())}),e=r?function(t){self!=top?l?t():o.push(t):function(){try{a.doScroll("left")}catch(n){return setTimeout(function(){e(t)},50)}t()}()}:function(n){l?n():o.push(n)});function f(n){for(l=1;n=o.shift();)n()}function s(){for(var a,n=window,r={};n;){try{if(n.frames.__tcfapiLocator){a=n;break}}catch(n){}if(n===window.top)break;n=n.parent}window.__tcfapi=function(n,t,e,o){a?(t={__tcfapiCall:{command:n,parameter:o,version:t,callId:o=Math.random()+""}},r[o]=e,a.postMessage(t,"*")):e({msg:"CMP not found"},!1)},window.addEventListener("message",function(n){var t={};try{t="string"==typeof n.data?JSON.parse(n.data):n.data}catch(n){}var e=t.__tcfapiReturn;e&&"function"==typeof r[e.callId]&&(r[e.callId](e.returnValue,e.success),r[e.callId]=null)},!1)}var m={addPixel:function(n,t){var e,o=["pixelId="+encodeURIComponent(n),v];for(e in t){var a=encodeURIComponent(t[e]);o.push(e+"="+a)}var r=o.join("&");(n=document.createElement("script")).async=!0,n.type="text/javascript",n.src="https://tracking.m6r.eu/pixel/container?"+r,document.getElementsByTagName("body")[0].appendChild(n)}},w=1e3,v="";function _(n,t,e){var o=!1,a=setTimeout(c,w),r=new Image;r.onload=c,r.onerror=c;var i=encodeURIComponent(t);function c(){window.clearTimeout(a),o||(o=!0,e())}v=t?"gdpr="+Number(n)+"&gdpr_consent="+i:"gdprFallback=referer",r.src="https://tracking.m6r.eu/consent/pixel?"+v}function h(){window._m6rq=window._m6rq||[];var n=window._m6rq;for(n.push=function(){var n=Array.prototype.push.apply(this,arguments);return p(h),n};e=n.shift();){var t=e.shift(),e=e;t in m&&m[t].apply(null,e)}window._m6rApiLoaded=(window._m6rApiLoaded||0)+1}p(function(){!function(e){window.__tcfapi||s();var o=setTimeout(function(){_(o=null,null,e)},1e3);window.__tcfapi("getTCData",2,function(n,t){o&&(clearTimeout(o),t&&n&&n.tcString?_(n.gdprApplies,n.tcString,e):_(null,null,e))})}(h)})}();