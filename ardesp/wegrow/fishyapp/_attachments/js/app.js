var LIMIT = 50;

function init() {
    var url = '/fishydata/_design/fishyapp/_view/data_by_seq';
    var q = {
        descending: true,
        include_docs: true,
        update_seq: true,
        limit: LIMIT
    }
    couchr.get(url, q, function (err, data) {
        if (err) {
            throw err;
        }
        var rows = data.rows;
        if (rows.length) {
            initGraph(rows, rows[0].key);
        }
        else {
            initGraph([], 0);
        }
    });
}

function renderTable(rows) {
    var tbody = $('#tableData tbody');
    var trs = $('tr', tbody);
    trs.each(function (i) {
        var tds = $('td', this);
        var row = rows[i];
        tds[0].textContent = row.key;
        tds[1].textContent = row.doc.timestamp;
        tds[2].textContent = row.doc.waterTemp;
    });
    var i = trs.length;
    var len = rows.length;
    while (i < len) {
        var row = rows[i];
        tbody.append(
            '<tr>' +
                '<td>' + row.key + '</td>' +
                '<td>' + row.doc.timestamp + '</td>' +
                '<td>' + row.doc.waterTemp + '</td>' +
            '</tr>'
        );
        i++;
    }
}

function rowsToGraphLabels(rows) {
    var seqs = rows.map(function (r) {
        return r.key;
    });
    seqs.reverse();
    return seqs;
}

function rowsToGraphSeries(rows) {
    var docs = rows.map(function (r) {
        return r.doc;
    });
    docs.reverse();             // rows are in descending order
    var waterTemps = docs.map(function (doc) {
        return doc.waterTemp;
    });
    return [
        waterTemps
    ];
}

function rowsToChartData(rows) {
    return {
        labels: rowsToGraphLabels(rows),
        series: rowsToGraphSeries(rows)
    };
}

function initGraph(rows, since) {
    // Our labels and three data series
    var data = rowsToChartData(rows);

    // We are setting a few options for our chart and override the defaults
    var options = {
        // high: 100,
        low: 0,
        showPoint: false,
        lineSmooth: true,
        axisX: {
            showGrid: false,
            showLabel: false
        },
        axisY: {
            offset: 60,
            labelInterpolationFnc: function(value) {
                return value + ' C';
            }
        }
    };

    // All you need to do is pass your configuration as third parameter to the chart function
    var graph = new Chartist.Line('#chart1', data, options);

    renderTable(rows);

    var feed = couchr.changes('/fishydata', {
        since: since,
        include_docs: true,
        filter: '_view',
        view: 'fishyapp/data_by_seq'
    });
    feed.on('change', function (change) {
        console.log(change);
	var row = {
	    key: change.seq,
	    doc: change.doc
	};
        // add to start of rows
	rows.unshift(row);
	while (rows.length > LIMIT) {
	    rows.pop();
	}
	renderTable(rows);
        graph.update(rowsToChartData(rows));
    });
}

/*
   var feed = couchr.changes(db_url);
   feed.on('change', function (change_object) { ...  });
   feed.on('error', function (err) { ...  });
   feed.pause();
   feed.resume();
   */

init();
