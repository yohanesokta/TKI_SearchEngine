<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>LinuxSearch - Explore the Open Source World</title>
    
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB" crossorigin="anonymous">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>

    <style>
        body {
            background-color: #f8f9fa;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            color: #202124;
            overflow-x: hidden;
        }

        #loading-bar {
            position: fixed; top: 0; left: 0; width: 100%; height: 3px;
            background-color: transparent; z-index: 9999; overflow: hidden; display: none;
        }
        #loading-bar::before {
            content: ''; position: absolute; top: 0; left: -50%; width: 50%; height: 100%;
            background-color: #0078d4; animation: loadingSlide 1s infinite linear;
        }
        @keyframes loadingSlide { 0% { left: -50%; } 100% { left: 100%; } }

        .search-header {
            background-color: #ffffff; border-bottom: 1px solid #ebebeb;
            padding: 15px 30px; position: sticky; top: 0; z-index: 1000;
        }
        .brand-logo { font-size: 1.5rem; font-weight: bold; color: #0078d4; text-decoration: none; margin-right: 30px; display: flex; align-items: center; gap: 8px; }
        .search-form-container { display: flex; align-items: center; background: #fff; border: 1px solid #dfe1e5; border-radius: 24px; padding: 5px 15px; box-shadow: 0 1px 6px rgba(32, 33, 36, 0.1); max-width: 750px; width: 100%; transition: box-shadow 0.3s ease; }
        .search-form-container:hover, .search-form-container:focus-within { box-shadow: 0 1px 8px rgba(32, 33, 36, 0.2); }
        .search-input { border: none; outline: none; box-shadow: none; flex-grow: 1; padding: 8px 10px; background: transparent; }
        .search-input:focus { box-shadow: none; background: transparent; }
        .search-limit { border: none; border-left: 1px solid #ebebeb; padding-left: 15px; margin-right: 10px; width: 65px; outline: none; background: transparent; color: #5f6368; font-size: 0.95rem; }
        .search-limit::-webkit-inner-spin-button, .search-limit::-webkit-outer-spin-button { -webkit-appearance: none; margin: 0; }
        .search-btn { background: transparent; border: none; color: #0078d4; font-size: 1.2rem; cursor: pointer; padding: 0 10px; }

        .main-container {
            max-width: 1400px;
            margin: 0 auto;
            padding: 30px;
            transition: all 0.3s ease;
        }

        .result-container { max-width: 800px; margin: 0 auto; transition: max-width 0.3s ease; }
        .result-card {
            background: transparent; border: 1px solid transparent; margin-bottom: 20px;
            position: relative; border-radius: 8px; cursor: pointer; transition: all 0.2s ease;
        }
        .result-card:hover { background-color: #ffffff; box-shadow: 0 2px 10px rgba(0,0,0,0.05); }
        
        .result-card.active-card {
            background-color: #ffffff;
            border-color: #0078d4;
            box-shadow: 0 4px 15px rgba(0, 120, 212, 0.15);
        }

        .result-img-wrapper { width: 130px; min-width: 130px; border-radius: 6px; overflow: hidden; }
        .result-img-wrapper img { width: 100%; height: 90px; object-fit: cover; background-color: #f1f3f4; border: 1px solid #ebebeb; }
        .result-content { padding-right: 60px; }
        .result-url { color: #006621; font-size: 0.85rem; margin-bottom: 4px; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; }
        .result-title { color: #1a0dab; font-size: 1.2rem; font-weight: 400; text-decoration: none; display: block; margin-bottom: 6px; line-height: 1.3; }
        .result-title:hover { text-decoration: underline; }
        .result-desc { color: #4d5156; font-size: 0.9rem; line-height: 1.5; display: -webkit-box; -webkit-line-clamp: 2; -webkit-box-orient: vertical; overflow: hidden; }
        .result-score { position: absolute; bottom: 10px; right: 15px; background-color: #f1f3f4; color: #5f6368; font-size: 0.75rem; padding: 3px 8px; border-radius: 4px; font-weight: 500; border: 1px solid #e0e0e0; }

        .preview-panel {
            background: #ffffff;
            border: 1px solid #dfe1e5;
            border-radius: 12px;
            padding: 24px;
            box-shadow: 0 4px 20px rgba(0,0,0,0.06);
            position: sticky;
            top: 100px;
            height: calc(100vh - 130px);
            overflow-y: auto;
            display: flex;
            flex-direction: column;
        }

        .preview-img {
            width: 100%;
            max-height: 250px;
            object-fit: contain;
            background-color: #f8f9fa;
            border-radius: 8px;
            margin-bottom: 20px;
            border: 1px solid #ebebeb;
        }

        .preview-title { font-size: 1.5rem; font-weight: 600; color: #202124; margin-bottom: 10px; }
        .preview-url { color: #006621; font-size: 0.9rem; margin-bottom: 20px; word-wrap: break-word; }
        .preview-desc { font-size: 1rem; color: #4d5156; line-height: 1.6; flex-grow: 1; }
        
        .btn-close-preview {
            position: absolute; top: 15px; right: 15px;
            background: #f1f3f4; border: none; border-radius: 50%;
            width: 32px; height: 32px; display: flex; align-items: center; justify-content: center;
            cursor: pointer; color: #5f6368; transition: background 0.2s;
        }
        .btn-close-preview:hover { background: #e8eaed; color: #202124; }
    </style>
</head>
<body>

    <div id="loading-bar"></div>

    <header class="search-header d-flex align-items-center">
        <a class="brand-logo" href="#"> LinuxSearch</a>
        <form action="#" method="GET" onsubmit="return false" class="search-form-container">
            <input type="text" class="form-control search-input" placeholder="Cari dokumentasi, rilis distro, setup server..." name="q" id="cari" autocomplete="off">
            <input type="number" class="form-control search-limit" name="rank" id="rank" value="10" min="1">
            <button class="search-btn" id="search" type="submit"><i class="fas fa-search"></i></button>
        </form>
    </header>

    <main role="main" class="main-container">
        <div class="row">
            
            <div class="col-lg-12 transition-width" id="results-wrapper">
                <div class="result-container" id="content">
                    <div class="text-muted mt-2 text-center" id="initial-message">
                        <p>Ketik perintah pencarian di atas untuk memulai.</p>
                    </div>
                </div>
            </div>

            <div class="col-lg-5 d-none" id="preview-wrapper">
                <div class="preview-panel">
                    <button class="btn-close-preview" id="close-preview" title="Tutup Preview"><i class="fas fa-times"></i></button>
                    
                    <img id="prev-img" src="" alt="Preview Gambar" class="preview-img" onerror="this.src='https://upload.wikimedia.org/wikipedia/commons/3/35/Tux.svg'">
                    <h3 id="prev-title" class="preview-title"></h3>
                    <div id="prev-url" class="preview-url"></div>
                    <div id="prev-desc" class="preview-desc"></div>
                    
                    <a id="prev-link" href="#" target="_blank" class="btn btn-primary mt-4 w-100 shadow-sm">
                        <i class="fas fa-external-link-alt me-2"></i> Buka Artikel Lengkap
                    </a>
                </div>
            </div>

        </div>
    </main>

    <script>
    async function fetchServe_new(cari, rank) {
        $('#loading-bar').show();
        $('#content').html('<div class="text-muted mt-4 text-center">Memproses kueri...</div>');
        
        tutupPreview();

        try {
            let response = await fetch(`/search?limit=${rank}&query=${cari}`);
            let data = await response.json();
            let content = "";

            if(data['results'] && data['results'].length > 0) {
                $('#results-wrapper .result-container').css('margin', '0 auto 0 0');

                data['results'].forEach(element => {
                    content += `
                    <div class="card result-card" 
                         data-title="${element['title']}" 
                         data-url="${element['url']}" 
                         data-image="${element['image']}" 
                         data-desc="${element['description']}">
                         
                        <div class="card-body p-3 d-flex gap-3">
                            <div class="result-img-wrapper">
                                <img src="${element['image']}" alt="Thumbnail" onerror="this.src='https://upload.wikimedia.org/wikipedia/commons/3/35/Tux.svg'">
                            </div>
                            
                            <div class="flex-grow-1 result-content">
                                <p class="result-url mb-0">${element['url']}</p>
                                <a target="_blank" href="${element['url']}" class="result-title prevent-preview">${element['title']}</a>
                                <p class="result-desc mb-0">${element['description']}</p>
                            </div>
                            
                            <div class="result-score"><i class="fas fa-bolt text-warning me-1"></i> ${element['score']}</div>
                        </div>
                    </div>`;
                });
            } else {
                content = `<div class="alert alert-light mt-4 text-center border">Tidak ada hasil untuk "<b>${cari}</b>".</div>`;
            }

            $('#content').html(content);
        } catch (error) {
            console.error(error);
            $('#content').html(`<div class="alert alert-danger mt-4 text-center"><i class="fas fa-exclamation-triangle"></i> Gagal terhubung ke API Backend.</div>`);
        } finally {
            $('#loading-bar').hide();
        }
    }

    function tutupPreview() {
        $('#results-wrapper').removeClass('col-lg-7').addClass('col-lg-12');
        $('#preview-wrapper').addClass('d-none');
        $('.result-card').removeClass('active-card');
        
        $('#results-wrapper .result-container').css('margin', '0 auto');
    }

    $(document).ready(function() {
        
        $("#search").click(function(){
            var cari = $("#cari").val();
            var rank = $("#rank").val() || 10;
            if(cari.trim() !== "") fetchServe_new(cari, rank);
        });

        $("#cari, #rank").keypress(function(e) {
            if(e.which == 13) $("#search").click();
        });

        $(document).on('click', '.result-card', function(e) {
            if ($(e.target).closest('.prevent-preview').length) return;

            const title = $(this).attr('data-title');
            const url = $(this).attr('data-url');
            const image = $(this).attr('data-image');
            const desc = $(this).attr('data-desc');

            $('#prev-title').text(title);
            $('#prev-url').text(url);
            $('#prev-desc').text(desc);
            $('#prev-link').attr('href', url);
            
            $('#prev-img').attr('src', image).on('error', function() {
                $(this).attr('src', 'https://upload.wikimedia.org/wikipedia/commons/3/35/Tux.svg');
            });

            $('#results-wrapper').removeClass('col-lg-12').addClass('col-lg-7');
            $('#preview-wrapper').removeClass('d-none');
            
            $('.result-card').removeClass('active-card');
            $(this).addClass('active-card');
        });

        $('#close-preview').click(function() {
            tutupPreview();
        });

        $(document).keyup(function(e) {
            if (e.key === "Escape") {
                tutupPreview();
            }
        });

    });
    </script>
</body>
</html>