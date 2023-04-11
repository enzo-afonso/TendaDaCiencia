const url = 'https://api.thingspeak.com/channels/12345/feeds.json?api_key=YOUR_API_KEY&results=1000';
const ctx = document.getElementById('chart').getContext('2d');

fetch(url)
	.then(response => response.json())
	.then(data => {
		const feeds = data.feeds;
		const temperatureData = [];
		const dateLabels = [];

		// Format data for chart
		feeds.forEach(feed => {
			const temperature = parseFloat(feed.field1);
			const date = new Date(feed.created_at);

			temperatureData.push(temperature);
			dateLabels.push(date.toLocaleString());
		});

		// Create chart
		const chart = new Chart(ctx, {
			type: 'line',
			data: {
				labels: dateLabels,
				datasets: [{
					label: 'Temperatura (°C)',
					data: temperatureData,
					fill: false,
					borderColor: 'blue',
					pointRadius: 0
				}]
			},
			options: {
				scales: {
					yAxes: [{
						ticks: {
							beginAtZero: true
						}
					}]
				}
			}
		});

		// Filter by date
		const filterButton = document.getElementById('btn-filter');
		filterButton.addEventListener('click', () => {
			const dateInput = document.getElementById('date');
			const selectedDate = dateInput.value;
			const filteredFeeds = feeds.filter(feed => {
				const date = new Date(feed.created_at);
				return date.toDateString() === new Date(selectedDate).toDateString();
			});
			const filteredTemperatureData = [];
			const filteredDateLabels = [];

			filteredFeeds.forEach(feed => {
				const temperature = parseFloat(feed.field1);
				const date = new Date(feed.created_at);

				filteredTemperatureData.push(temperature);
				filteredDateLabels.push(date.toLocaleString());
			});

			chart.data.datasets[0].data = filteredTemperatureData;
			chart.data.labels = filteredDateLabels;
			chart.update();
		});
	});
