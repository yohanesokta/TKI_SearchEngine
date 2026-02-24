<!DOCTYPE html>
<html lang="{{ str_replace('_', '-', app()->getLocale()) }}">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">

    <title>SearchBook</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB" crossorigin="anonymous">
    
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>

</head>
<body>

    <header>
        <nav style="background-image: linear-gradient( 111.4deg,  rgba(122,192,233,1) 18.8%, rgba(4,161,255,1) 100.2% );" class="navbar navbar-expand-lg navbar-light bg-white shadow-sm rounded">
            <a class="navbar-brand" href="#">Book Searching</a>
        </nav>
    </header>

    <main role="main" style="height:200px; background-image: linear-gradient( 111.4deg,  rgba(122,192,233,1) 18.8%, rgba(4,161,255,1) 100.2% );">
        <div class="container pt-5">
            <!-- Another variation with a button -->
            <form action="#" method="GET" onsubmit="return false">
            <div class="input-group">
                <input type="text" class="form-control" placeholder="Search the Book" name="q" id="cari">
                <div class="col-lg-1">
                <select class="form-control" name="rank" id="rank">
                    <option value="5">5</option>
                    <option value="10">10</option>
                    <option value="20">20</option>
                  </select>
                </div>
                <div class="input-group-append">
                <input class="btn btn-secondary fas fa-search" id="search" type="submit" value="Search">
                </div>
            </div>
        </form>
        </div>
    </main>

    <div class="row m-4" id="content">
     
    </div>
    <script>
    async function fetchServe_new (cari,rank) {
        try {
            let data =  await fetch(`http://127.0.0.1:5000/search?n=${rank}&query=${cari}`)
            data = await data.json();
            let content = ""
            data.forEach(element => {
                content += `<div class='col-lg-5'>
                <div class='card mb-2'>
                    <div style='display: flex; flex: 1 1 auto;'>
                        <div class='img-square-wrapper'>
                            <img src='http://books.toscrape.com/${element['image']}'>
                        </div>
                        <div class='card-body'>
                            <h6 class='card-title'><a target='_blank' href='http://books.toscrape.com/catalogue/${element['price']}'>${element['title']}</a></h6>
                            <p class='card-text text-success'>Price : ${element['price']}</p>
                            <span style='color:#424242; font-size:10pt;'>Score : ${element['score']} </span>
                        </div>
                    </div>
                </div>
            </div>`
            });

            $('#content').html(content);
        } catch (error) {
            console.error(error)
        }
    }

    $(document).ready(function() {
        $("#search").click(function(){
            var cari = $("#cari").val();
            var rank = $("#rank").val();
            
            fetchServe_new(cari,rank);
            // $.ajax({
            //     url:'/search?q='+cari+'&rank='+rank,
            //     dataType : "json",
            //     success: function(data){
            //              $('#content').html(data);
            //         },
            //         error: function(data){
            //             alert("Please insert your command");
            //         }
            // });
        });
    });
</script>
</body>
</html>