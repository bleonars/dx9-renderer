#pragma once

namespace Utils {

    // safely release COM interface
    template< typename t > FORCEINLINE void safe_release( t **com ) {
        if( com && *com ) {
            ( *com )->Release();
            ( *com ) = nullptr;
        }
    }

}