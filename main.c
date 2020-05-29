#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t berber;//Berber semaforumuzu tan�mlad�k.

void *Berber();//Fonksiyon prototiplerimizi tan�mlad�k
void *Musteri();

//Kulland���m�z de�i�kenleri tan�mlad�k
int gelenMusteri = 0;//O an gelen m��terinin g�nl�k s�ras�
int bosKoltuk =5;//D�kkanda bo� bulunan koltuk say�s�.5 i ge�emez.
int bekleyenMusteriler[5];//Bekleme salonunda bulunan m��terilerin s�rayla tutuldu�u dizi(kuyruk �eklinde)
int siradakiMusteri = -1;//Berberin salonda s�radaki m��teriyi bulmas� i�in kulland�k
int sonGelenMusteri = -1;//Yeni gelen m��terinin dizide s�radaki indise yerle�mesi i�in kulland�k

int main()
{
    srand(time(NULL));//Her defas�nda farkl� de�er �retmek i�in
    pthread_t berberThread,musteriThread[20];//Thread tan�mlamalar�m�z� yapt�k, 1 Berber 20 M��teri Thread'i

    sem_init(&berber,1,0);//Berber semaforumuzu burda olu�turduk.

    pthread_create(&berberThread,NULL,Berber,NULL);//Berber thread imizi olu�turduk.
    int j=0;
    for(j=0;j<20;j++)
    {
        pthread_create(&musteriThread[j],NULL,Musteri,NULL);//M��teri threadlerimizi olu�turduk
    }

    pthread_join(berberThread,NULL);//Join i�lemini ayr�ca yapt�k.
    for(j=0;j<20;j++)
    {
        pthread_join(musteriThread[j],NULL);
    }


    return 0;
}

void *Musteri()
{


        int sure = 1 + rand()%20;//M��terilerin rastgele s�relerde d�kkana gelmesini sa�lad�k.
        sleep(sure);

        gelenMusteri++;//O s�rada d�kkana yeni gelen m��terinin numaras�n� ayarlad�k.
        printf("%d. musteri dukkana geldi.\n",gelenMusteri);
        if(bosKoltuk==5)//E�er o s�rada b�t�n koltuklar bo�sa berber uyand�r�l�yor.
        {
            sem_post(&berber);
            printf("Berber uyandi\n");
        }

        if(bosKoltuk>0)//M��teri d�kkandaki bo� koltu�a oturuyor ve bekleyenMusteriler dizisine yerle�tiriliyor.
        {
            sonGelenMusteri = (sonGelenMusteri+1)%5;//Yeni gelen m��terinin dizinin bir sonraki indisine yerle�mesini sa�lad�k.
            bosKoltuk--;
            bekleyenMusteriler[sonGelenMusteri] = gelenMusteri;

            printf("%d. musteri bekleme salonuna oturdu.\n",gelenMusteri);

        }
        else//B�t�n koltuklar doluysa m��teri d�kkandan ayr�l�yor.
        {
            printf("%d. musteri yer bulamadi.Dukkandan ayriliyor.\n",gelenMusteri);
        }


}

void *Berber()
{
    while(1)
    {
        //printf("Bos koltuk sayisi : %d \n",bosKoltuk);

        if(bosKoltuk==5)//Salonun dolu olup olmad���n� kontrol ettik. Bo� ise sem_wait ile berberin uyumas�n� sa�lad�k.
        {
            printf("Berber uyumaya gitti\n");
            sem_wait(&berber);
        }

        siradakiMusteri = (siradakiMusteri + 1) % 5;//Berberin sa��n� kesece�i s�radaki m��teriyi dizinin bir sonraki indisinden bulduk. Kuyruk yap�s� oldu�u i�in mod ald�k.


        printf("berber %d. musterinin sacini kesmeye basladi.\n",bekleyenMusteriler[siradakiMusteri]);

        sleep(1);//Berberin m��teriyi 1 sn de tra� etmesini sa�lad�k
        printf("berber %d. musterinin sacini kesmeyi bitirdi\n",bekleyenMusteriler[siradakiMusteri]);
        bosKoltuk++;//Koltu�u bo�alt�yoruz.

    }
}


