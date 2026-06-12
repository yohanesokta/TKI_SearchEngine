document.addEventListener('DOMContentLoaded', () => {
    const searchForm = document.getElementById('search-form');
    const searchInput = document.getElementById('search-input');
    const kInput = document.getElementById('k-input');
    const resultsContainer = document.getElementById('results-container');
    const resultsInfo = document.getElementById('results-info');
    const resultCount = document.getElementById('result-count');
    const queryDisplay = document.getElementById('query-display');
    const loader = document.getElementById('loader');

    searchForm.addEventListener('submit', async (e) => {
        e.preventDefault();
        const query = searchInput.value.trim();
        const k = kInput.value;

        if (!query) return;

        // Show loader
        loader.classList.remove('hidden');

        try {
            const response = await fetch(`/search?q=${encodeURIComponent(query)}&k=${k}`);
            if (!response.ok) throw new Error('Search failed');

            const results = await response.json();
            displayResults(results, query);
        } catch (error) {
            console.error('Error:', error);
            resultsContainer.innerHTML = `<div class="empty-state"><p style="color: red;">Error: ${error.message}</p></div>`;
        } finally {
            loader.classList.add('hidden');
        }
    });

    function displayResults(results, query) {
        resultsInfo.classList.remove('hidden');
        resultCount.textContent = results.length;
        queryDisplay.textContent = query;

        if (results.length === 0) {
            resultsContainer.innerHTML = `
                <div class="empty-state">
                    <p>No results found for your query. Try different keywords.</p>
                </div>`;
            return;
        }

        resultsContainer.innerHTML = results.map(result => `
            <article class="result-card">
                <div class="result-header">
                    <span class="doc-id">DOC ID: ${result.doc_id}</span>
                    <span class="score-badge">Similarity: ${(result.score * 100).toFixed(4)}%</span>
                </div>
                <p class="result-text">${result.text}</p>
            </article>
        `).join('');
    }
});
